#define WLR_USE_UNSTABLE

#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/desktop/Window.hpp>
#include <hyprland/src/plugins/PluginAPI.hpp>
#include <hyprland/src/protocols/XDGShell.hpp>

#include "ToplevelUtils.hpp"

#define PLUGIN_NAME "hypruntile"
#define MSG_PREFIX "[" PLUGIN_NAME "] "

using namespace std::string_view_literals;

inline HANDLE PHANDLE = nullptr;

// Do NOT change this function.
APICALL EXPORT std::string PLUGIN_API_VERSION() {
    return HYPRLAND_API_VERSION;
}

void applyHypruntileRules(CWindow& window) {
    bool untile = false;
    bool unmaximize = false;
    for (auto const& r : window.m_matchedRules) {
        if (r->m_ruleType != CWindowRule::RULE_PLUGIN)
            continue;
        std::string_view ruleName { r->m_rule };
        ruleName.remove_prefix("plugin:"sv.length());
        if (!ruleName.starts_with(PLUGIN_NAME ":"))
            continue;
        ruleName.remove_prefix(PLUGIN_NAME ":"sv.length());
        if (ruleName.starts_with("untile")) {
            untile = true;
        } else if (ruleName.starts_with("unmaximize")) {
            unmaximize = true;
        }
    }
    ToplevelUtils::setTiled(*window.m_xdgSurface->m_toplevel, !untile);
    window.m_xdgSurface->m_toplevel->setMaximized(!unmaximize);
}

void onNewWindow(void* self, std::any data) {
    const auto PWINDOW = std::any_cast<PHLWINDOW>(data);
    applyHypruntileRules(*PWINDOW);
}

void onWindowRulesUpdated(void* self, std::any data) {
    const auto PWINDOW = std::any_cast<PHLWINDOW>(data);
    applyHypruntileRules(*PWINDOW);
}

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle) {
    PHANDLE = handle;

    const std::string HASH = __hyprland_api_get_hash();

    if (HASH != GIT_COMMIT_HASH) {
        HyprlandAPI::addNotification(PHANDLE, MSG_PREFIX "Failure in initialization: Version mismatch (headers ver is not equal to running hyprland ver)",
                                     CHyprColor{1.0, 0.2, 0.2, 1.0}, 5000);
        throw std::runtime_error(MSG_PREFIX "Version mismatch");
    }

    // Apply rules to all existing windows.
    for (auto& w : g_pCompositor->m_windows) {
        if (w->isHidden() || !w->m_isMapped)
            continue;
        applyHypruntileRules(*w);
    }

    static auto P1 = HyprlandAPI::registerCallbackDynamic(PHANDLE, "openWindow", [&](void* self, SCallbackInfo& info, std::any data) { onNewWindow(self, data); });
    static auto P2 = HyprlandAPI::registerCallbackDynamic(PHANDLE, "windowUpdateRules", [&](void* self, SCallbackInfo& info, std::any data) { onWindowRulesUpdated(self, data); });

    HyprlandAPI::addNotification(PHANDLE, MSG_PREFIX "Initialized successfully!", CHyprColor{0.2, 1.0, 0.2, 1.0}, 5000);

    return {PLUGIN_NAME, "A plugin to remove XDG_TOPLEVEL_STATE_TILED_* flags from certain windows.", "Equaraphanus", "0.1"};
}

APICALL EXPORT void PLUGIN_EXIT() {
    // Restore all windows.
    for (auto& w : g_pCompositor->m_windows) {
        if (w->isHidden() || !w->m_isMapped)
            continue;
        ToplevelUtils::setTiled(*w->m_xdgSurface->m_toplevel, true);
        w->m_xdgSurface->m_toplevel->setMaximized(true);
    }
}

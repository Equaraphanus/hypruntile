#include <hyprland/src/plugins/PluginAPI.hpp>
#define private public // To access m_resource to be able to check its version.
#include <hyprland/src/protocols/XDGShell.hpp>
#undef private

#include "ToplevelUtils.hpp"

namespace ToplevelUtils {

void setTiled(CXDGToplevelResource& toplevel, bool tiled) {
    // This check mirrors the one in the CXDGToplevelResource constructor.
    if (toplevel.m_resource->version() < 2)
        return;

    auto& pendingApply = toplevel.m_pendingApply;
    bool set = std::ranges::find_if(toplevel.m_pendingApply.states, isToplevelStateTiled) != pendingApply.states.end();

    if (tiled == set)
        return;

    if (tiled) {
        pendingApply.states.push_back(XDG_TOPLEVEL_STATE_TILED_LEFT);
        pendingApply.states.push_back(XDG_TOPLEVEL_STATE_TILED_RIGHT);
        pendingApply.states.push_back(XDG_TOPLEVEL_STATE_TILED_TOP);
        pendingApply.states.push_back(XDG_TOPLEVEL_STATE_TILED_BOTTOM);
    } else {
        std::erase_if(pendingApply.states, isToplevelStateTiled);
    }
    toplevel.applyState();
    toplevel.m_owner->scheduleConfigure();
}

} // namespace ToplevelUtils

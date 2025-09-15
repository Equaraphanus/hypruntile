#include <hyprland/src/protocols/XDGShell.hpp>

namespace ToplevelUtils {

inline constexpr bool isToplevelStateTiled(xdgToplevelState state) {
    return state == XDG_TOPLEVEL_STATE_TILED_LEFT || state == XDG_TOPLEVEL_STATE_TILED_RIGHT || state == XDG_TOPLEVEL_STATE_TILED_TOP || state == XDG_TOPLEVEL_STATE_TILED_BOTTOM;
}

void setTiled(CXDGToplevelResource&, bool);

} // namespace ToplevelUtils

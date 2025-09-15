# hypruntile

Allows you to unset the `XDG_TOPLEVEL_STATE_TILED_*` flags from certain windows.

Example Config:
```
# Remove the XDG_TOPLEVEL_STATE_TILED_* and XDG_TOPLEVEL_STATE_MAXIMIZED
windowrule = plugin:hypruntile:untile, class:mpv, floating:1
windowrule = plugin:hypruntile:unmaximize, class:mpv, floating:1
```

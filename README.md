# hypruntile

Allows you to unset the `XDG_TOPLEVEL_STATE_TILED_*` and `XDG_TOPLEVEL_STATE_MAXIMIZED` flags Hyprland adds to all windows by default from certain windows. See [this discussion](https://github.com/hyprwm/Hyprland/discussions/11723) for details.

Example Config:
```
# Allow floating MPV to resize itself
windowrule = plugin:hypruntile:untile, class:mpv, floating:1
windowrule = plugin:hypruntile:unmaximize, class:mpv, floating:1
```

## Acknowledgements

Practically everything in this repository is borrowed from Hyprland's sources.

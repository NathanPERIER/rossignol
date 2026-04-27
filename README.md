
# Rossignol

## Build

```bash
meson setup --native-file debug.ini build_debug
meson setup --native-file release.ini build_release

cd build_debug
ninja
```

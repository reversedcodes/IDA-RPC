# IDA Discord RPC

IDA Discord RPC is a plugin for IDA that displays your current activity as Discord Rich Presence. It enhances your Discord profile with real-time information about what you're working on inside IDA.

![License: Apache 2.0](https://img.shields.io/badge/License-Apache_2.0-blue.svg)

## Features

- Shows your current function, view type (Disassembly, Pseudocode, Hex View, ...) and file name
- Shows the IDA edition + version and elapsed time
- Debugging status (`Debugging <file>` / `Paused` / `Running`)
- Idle detection after a configurable timeout
- Privacy mode: hide the real file name
- Customizable text templates with placeholders: `{function}` `{view}` `{file}` `{address}` `{version}`
- Everything configurable from a native settings dialog with live connection status
- Top-level **Discord** menu in the menubar: `Settings...` and `Reconnect to Discord`

## Compatibility

- IDA Pro / IDA Home 9.0 - 9.5, with a dedicated build for every IDA version
- Windows x86-64 (`.dll`), Linux x86-64 (`.so`), macOS x86-64/arm64 (`.dylib`)

## Installation

### Using HCLI (recommended)

The plugin ships [IDA plugin metadata](ida-plugin.json), so the [Hex-Rays CLI](https://hcli.docs.hex-rays.com/) plugin manager can install it, pick the build for your IDA version and platform and keep it updated:

```bash
hcli plugin install ida-discord-rpc
```

Managing the installation:

```bash
hcli plugin status
hcli plugin upgrade ida-discord-rpc
hcli plugin uninstall ida-discord-rpc
```

If HCLI is not installed yet, on macOS and Linux:

```bash
curl -LsSf https://hcli.docs.hex-rays.com/install | sh
```

On Windows, in PowerShell:

```powershell
iwr -useb https://hcli.docs.hex-rays.com/install.ps1 | iex
```

### Manual

Every [release](https://github.com/reversedcodes/IDA-RPC/releases) has one ZIP per IDA version, named `ida-discord-rpc-<IDA version>.zip` (for example `ida-discord-rpc-9.5.zip`). Download the one for your IDA version and either install it with `hcli plugin install ./ida-discord-rpc-9.5.zip` or copy the binary for your OS (`ida-discord-rpc.dll`, `.so` or `.dylib`) into your IDA plugins directory:

| OS | Plugins directory |
| --- | --- |
| Windows | `%APPDATA%\Hex-Rays\IDA Pro\plugins` |
| Linux | `~/.idapro/plugins` |
| macOS | `~/Library/Application Support/Hex-Rays/IDA Pro/plugins` |

Restart IDA afterwards. Configure the plugin via the **Discord** menu in the menubar.

Releases before 1.3 named the binary `ida_discord_rpc`. Delete that file from your plugins directory when you update by hand, otherwise IDA loads both.

## Building from source

```bash
git clone https://github.com/reversedcodes/IDA-RPC
cd IDA-RPC
git clone --depth 1 --branch v9.5.0-release https://github.com/HexRaysSA/ida-sdk lib/ida-sdk
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Check out the SDK tag of the IDA version you build for (`v<IDA version>.0-release`, 9.0 to 9.5). The plugin is written to `build/ida_plugins/ida-discord-rpc.<so|dll|dylib>`.

On macOS, set `IDASDK_ROOT` to your IDA installation (e.g., `/Applications/IDA Professional 9.5.app`) or pass `-DIdaSDK_LIBRARY=/path/to/libida.dylib`.

The plugin is written against the open-source [IDA SDK](https://github.com/HexRaysSA/ida-sdk); Discord integration uses [discord-rpc](https://github.com/discord/discord-rpc) (fetched automatically).

### Project layout

```
ida-plugin.json            plugin metadata: name, version, supported IDA versions
CMakeLists.txt             project setup, IDA SDK and discord-rpc lookup
cmake/                     FindIdaSDK and discord-rpc fetch modules
src/ida_discord_rpc/       plugin sources
  plugin.cpp               plugin descriptor, the entry point IDA loads
  plugin_mod.*             plugin lifecycle: startup, timer, shutdown
  discord/                 Discord connection and presence rendering
  gui/                     settings storage, settings dialog, Discord menu
  listener/                IDA event listeners that report activity
  util/                    logging and IDA helpers
.github/                   build and release workflow, packaging script
```

### Releases

`ida-plugin.json` drives the release. Pushing a tag that names its `version` (tag `1.3` for version `1.3.0`) builds the plugin against the SDK of every IDA version listed in `idaVersions`, on Windows, Linux and macOS, and publishes one archive per IDA version: `ida-discord-rpc-<IDA version>.zip`. To support a new IDA version, add it to `idaVersions`.

## License

This project is licensed under the [Apache License 2.0](LICENSE).
You are free to use, modify, and distribute this software under the terms of the license.

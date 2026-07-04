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

- IDA Pro / IDA Home 9.x
- Windows x86-64 (`.dll`) and Linux x86-64 (`.so`)

## Installation

Download the ZIP from the [latest release](https://github.com/reversedcodes/IDA-RPC/releases) and copy the binary for your OS into your IDA plugins directory:

| OS | Plugins directory |
| --- | --- |
| Windows | `%APPDATA%\Hex-Rays\IDA Pro\plugins` |
| Linux | `~/.idapro/plugins` |

Restart IDA afterwards. Configure the plugin via the **Discord** menu in the menubar.

## Building from source

```bash
git clone https://github.com/reversedcodes/IDA-RPC
cd IDA-RPC
git clone https://github.com/HexRaysSA/ida-sdk lib/ida-sdk
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

The plugin is written against the open-source [IDA SDK](https://github.com/HexRaysSA/ida-sdk); Discord integration uses [discord-rpc](https://github.com/discord/discord-rpc) (fetched automatically).

## License

This project is licensed under the [Apache License 2.0](LICENSE).
You are free to use, modify, and distribute this software under the terms of the license.

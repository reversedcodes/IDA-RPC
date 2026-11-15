#!/usr/bin/env bash
# Assemble the release archives: one HCLI plugin archive per supported IDA version.
#
#   package.sh <artifacts-dir> <output-dir>
#
# <artifacts-dir> holds the build outputs, one directory per IDA version and
# platform, laid out the way actions/download-artifact unpacks them:
#
#   <artifacts-dir>/<plugin>-<ida version>-<platform>/<plugin>.<so|dll|dylib>
#
# Every entry of "idaVersions" in ida-plugin.json yields <plugin>-<ida version>.zip
# holding the binaries built against that version's SDK and a manifest narrowed
# to that single version, so HCLI installs exactly the build matching the user's IDA.
set -euo pipefail

usage="usage: package.sh <artifacts-dir> <output-dir>"
artifacts="${1:?$usage}"
output="${2:?$usage}"

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
manifest="$root/ida-plugin.json"
plugin="$(jq -r '.plugin.entryPoint' "$manifest")"
logo="$(jq -r '.plugin.logoPath' "$manifest")"

# <platform>:<binary extension>; the macOS dylib is a universal (arm64 + x86_64) binary.
binaries=(linux-x86_64:so windows-x86_64:dll macos-universal:dylib)

mkdir -p "$output"
output="$(cd "$output" && pwd)"
staging="$(mktemp -d)"
trap 'rm -rf "$staging"' EXIT

jq -r '.plugin.idaVersions[]' "$manifest" | while read -r ida; do
    package="$staging/$ida"
    mkdir -p "$package/$(dirname "$logo")"

    jq --arg ida "$ida" '.plugin.idaVersions = [$ida]' "$manifest" > "$package/ida-plugin.json"
    cp "$root/readme.md" "$package/README.md"
    cp "$root/$logo" "$package/$logo"

    for entry in "${binaries[@]}"; do
        platform="${entry%%:*}"
        extension="${entry##*:}"
        binary="$artifacts/$plugin-$ida-$platform/$plugin.$extension"
        if [[ ! -f "$binary" ]]; then
            echo "error: missing build output for IDA $ida ($platform): $binary" >&2
            exit 1
        fi
        cp "$binary" "$package/"
    done

    archive="$output/$plugin-$ida.zip"
    rm -f "$archive"
    (cd "$package" && zip -qr "$archive" .)
    echo "packaged $(basename "$archive")"
done

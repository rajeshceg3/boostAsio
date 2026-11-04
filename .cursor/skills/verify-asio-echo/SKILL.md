---
name: verify-asio-echo
description: Build and drive boostAsio's actual synchronous TCP echo server with isolated ports, byte-level clients and startup-failure evidence.
---
# Verify Asio echo
## Launch
Primary surface main.cpp synchronous loopback TCP server, one read_some up to1024bytes then Echo prefix response and close. Default ./main still127.0.0.1:8080; optional decimal port0-65535 selects another,0 asks OS for ephemeral. Actual assigned port prints only after successful bind. No auth/database. g++, make, pthread and Boostheaders required. Replit uses Clang/Boost from replit.nix; actual proof uses g++ with isolated UbuntuBoost1.74headers. Replit/device/otherplatform not certified. Never run shipped Nix-loader binary or overwrite it. Helper builds actual repo Makefile/source in unique tempfile.
## Doctor
`python3 .cursor/skills/verify-asio-echo/helpers/verify.py doctor --include /tmp/boost-verification-runtime/usr/include --out .verification-evidence/asio`
Set --include to directory containing boost/asio.hpp or BOOST_INCLUDE. CXX override supported, defaultg++. Read-only prerequisite/source check, no connection to8080.
## Drive
`python3 .cursor/skills/verify-asio-echo/helpers/verify.py drive --include /tmp/boost-verification-runtime/usr/include --out .verification-evidence/asio`
Builds actual source, tests4invalidargument cases and owned-port collision failure, starts owned server on0, parses live assigned port, sends3realTCPclients (text, newline, binary) with exact byte/EOF checks plus empty EOF disconnect. Uses process liveness and startup line before every client, never probes shared8080. Inspect features/README.md.
## Evidence
JSON actual payload/response hex, exit statuses, build.log and server.log. NonUTF8 server logbytes use backslash escapes, not silently dropped. Both doctor and drive must pass. This proves small single-read messages, not TCP framing, full1024+payload, concurrency, throughput, TLS or remote public networking. Larger/fragmented messages may be truncated under inherited single-read design; keep that limitation open.
## Cleanup
Kills/reaps exact owned child even on error, removes unique tempbuild only, checks gitstatus unchanged. Evidence remains. No name-based kills, deleting tracked main/cache or makeclean in source.
## Helpers
Executable helpers/verify.py uses Python3stdlib/make/compiler/actualBoost. No network dependency install in helper. Missing prerequisites are not app bugs. Use /maintain-verification-skill as source changes.

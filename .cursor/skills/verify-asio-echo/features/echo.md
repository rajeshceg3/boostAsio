# TCP echo and EOF
## Sub-features
Accept local client, read up to1024bytes once, Echo prefix, close socket, continue accepting nextclient. Empty cleanEOF produces no response/error.
## How to get to it (user POV)
Build source, run main with optionalport, connectTCPclient to printedloopbackport. Send smallmessage and readuntilclose.
## Driving it with verify.py
Drive owns actual server and3sequentialsocketclients, verifies exact text/newline/binary echo bytes andEOF, then an empty half-close. Serverlog and JSON survive.
## Gotchas
TCP is a stream and read_some is not message framing. Passing smallpayloads does not certify split/large/concurrentmessages. No TLS/auth/prod load. Default8080 may belong to someoneelse, never use it for tests. Shippedbinary hasNixloader; rebuild actualsource.

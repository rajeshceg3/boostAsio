# Port selection and failures
## Sub-features
Default8080 preserved, optional0-65535decimalport, actual assigned port printed after bind; invalidargs/busyport return1with Server error diagnostic.
## How to get to it (user POV)
Run main0 for ownedephemeralport or main12345 for explicitport. Invalid values fail beforeacceptingclients.
## Driving it with verify.py
4invalidargs tested; separate bound/listening socket held byhelper tests occupiedport without sendinguserdata to it. Start own child on0 and readits portline before clients.
## Gotchas
Original hardcoded8080 prevented safeparallelproof when alreadyoccupied inenvironment. Optionalport is product isolation improvement, not authorization to connect to anyexistinglistener. Default8080runtime is not tested here; sourcepreserves it. No signalhandler/gracefulshutdownclaim, helperusesSIGTERMownedchild.

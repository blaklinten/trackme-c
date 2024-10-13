#!/bin/bash

cd ../web/ || exit

export TRACKME_LOG_LEVEL=3
CMD='../build/main'

declare SERVER_PID

reload() {
  echo "Restarting server with PID=$SERVER_PID"
  kill "$SERVER_PID"
  "$CMD" & SERVER_PID="$!"
}

trap reload SIGINT 

"$CMD" & SERVER_PID="$!"
echo "Server started with PID=$SERVER_PID"

while true; do
  read -sr -n1
  if [[ "$REPLY" == "q" ]]; then
    kill "$SERVER_PID"
    break
  else
    echo "You pressed ['$REPLY']. Use 'q' to quit"
  fi
done

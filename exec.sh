#! /bin/bash
make main
if [ $? -eq 0 ]; then

    nohup ./main > main.out  2>&1 &

else

    echo "make failed"

fi


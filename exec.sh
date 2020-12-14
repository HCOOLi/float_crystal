#! /bin/bash
make main
if [ $? -eq 0 ]; then
     ./main 12 $1 
else
    echo "make failed"
fi


#!/bin/bash
multiply()
{
    if [ $1 -gt 1 ]; then
        result=`expr $result \* $1`
        next=`expr $1 - 1`
        multiply $next
    fi
}
if [ $# -ne 1 ]; then
      echo -e "Please input a parameter!\nUsage: $0 [n]"
      exit 1
fi
result=1
if [ $1 -eq 1 ]; then
      :
elif [ $1 -gt 1 ]; then
      multiply $1
else
      echo "Invalid input parameter! It MUST be a integer greater than 0."
      exit 1
fi
echo "$1! = $result"
exit 0

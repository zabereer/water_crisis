Finding steps to measure water
==============================

Next time John McClane is confronted with measuring water to stop a bomb, use this:

```
./water_crisis --help
Determine how to measure amount(s) of water using buckets of different capacities.
Add buckets and specify amount to measure with arguments below.
An absent measure would output ways to measure all amounts up to total capacity.
Arguments are: -a | --add-bucket    add a bucket of specific capacity - repeat for each
               -m | --measure       specify amount of water to measure
               -s | --max-steps     specify maximum allowed steps, defaults to 100
               -h | --help          output this message

Buckets will be named from A(n), B(n), C(n) etc where n is the bucket's capacity
User will be asked to extend number of steps if they are reached.
Normally this will happen if no odd size bucket was specified.
```

Given a bucket with 3 litres capacity and another with 5, determine the steps required to measure 4 litres. 
Swap the order in which buckets are specified to see how wastage is affected.

```
./water_crisis --add-bucket=5 --add-bucket=3 --measure=4
For measuring 4 :
  Fill bucket A(5) from 0 to 5
  Transfer bucket A(5) into B(3) reducing A(5) from 5 to 2 and increasing B(3) from 0 to 3
  Empty bucket B(3) from 3 to 0
  Transfer bucket A(5) into B(3) reducing A(5) from 2 to 0 and increasing B(3) from 0 to 2
  Fill bucket A(5) from 0 to 5
  Transfer bucket A(5) into B(3) reducing A(5) from 5 to 4 and increasing B(3) from 2 to 3
  Empty bucket B(3) from 3 to 0
  Wasted water: 6
```

Or to cover all possible measurements:

```
./water_crisis --add-bucket=3 --add-bucket=5
For measuring 1 :
  Fill bucket A(3) from 0 to 3
  Transfer bucket A(3) into B(5) reducing A(3) from 3 to 0 and increasing B(5) from 0 to 3
  Fill bucket A(3) from 0 to 3
  Transfer bucket A(3) into B(5) reducing A(3) from 3 to 1 and increasing B(5) from 3 to 5
  Empty bucket B(5) from 5 to 0
  Wasted water: 5

For measuring 2 :
  Fill bucket B(5) from 0 to 5
  Transfer bucket B(5) into A(3) reducing B(5) from 5 to 2 and increasing A(3) from 0 to 3
  Empty bucket A(3) from 3 to 0
  Wasted water: 3

For measuring 3 :
  Fill bucket A(3) from 0 to 3
  Wasted water: 0

For measuring 4 :
  Fill bucket A(3) from 0 to 3
  Transfer bucket A(3) into B(5) reducing A(3) from 3 to 0 and increasing B(5) from 0 to 3
  Fill bucket A(3) from 0 to 3
  Transfer bucket A(3) into B(5) reducing A(3) from 3 to 1 and increasing B(5) from 3 to 5
  Empty bucket B(5) from 5 to 0
  Transfer bucket A(3) into B(5) reducing A(3) from 1 to 0 and increasing B(5) from 0 to 1
  Fill bucket A(3) from 0 to 3
  Wasted water: 5

For measuring 5 :
  Fill bucket B(5) from 0 to 5
  Wasted water: 0

For measuring 6 :
  Fill bucket A(3) from 0 to 3
  Transfer bucket A(3) into B(5) reducing A(3) from 3 to 0 and increasing B(5) from 0 to 3
  Fill bucket A(3) from 0 to 3
  Wasted water: 0

For measuring 7 :
  Fill bucket B(5) from 0 to 5
  Transfer bucket B(5) into A(3) reducing B(5) from 5 to 2 and increasing A(3) from 0 to 3
  Empty bucket A(3) from 3 to 0
  Transfer bucket B(5) into A(3) reducing B(5) from 2 to 0 and increasing A(3) from 0 to 2
  Fill bucket B(5) from 0 to 5
  Wasted water: 3

For measuring 8 :
  Fill bucket A(3) from 0 to 3
  Fill bucket B(5) from 0 to 5
  Wasted water: 0
```

To build:   `cmake . && make`

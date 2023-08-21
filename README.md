# Simulated Annealing
### Organising people algorithmically

Simulated annealing is an optimisation algorithm that can be used for complex relationships. 

It's modelled around how atoms in a liquid state can organise themselves into the lowest possible energy state as the temperature of the fluid cools and solidifies.

The algorithm is rather simple to implement, you only need to define an initial starting temperature and a method of calculating energy of the overall system.

In this example I have a collection of 8 people that want to be split into 2 groups. Each person has a different relationship with eachother, some may be best of friends while some don't like eachother at all. The goal is to organise the group such that everybody is happy, and people who are closest friends should be positioned next to eachother. 

The following file is the initial data that is to be optimised

__parings.txt__
```
# alex and claire are partners
# florence and dave hate each other
# ebeneezer and bob are good friends
# dave knows alex through hector
# hector and alex are good friends
# claire is best friends with florence
# gerald doesn't like dave
alex,       bob,        claire,     dave,       ebeneezer,  florence,   gerald,     hector
-,          0,           100,       25,         0,          0,          0,          100  # alex
0,          -,           0,         0,          50,         0,          0,          0    # bob
100,        0,           -,         0,          0,          100,        0,          0    # claire
25,         0,           0,         -,          0,          -100,       -50,        50   # dave
0,          50,          0,         0,          -,          0,          0,          0    # ebeneezer
0,          0,           100,       -100,       0,          -,          0,          0    # florence
0,          0,           0,         -50,        0,          0,          -,          0    # gerald
100,         0,          0,         50,         0,          0,          0,          -    # hector
```

The numbers `100`, `50`, `0`, `-50` and `-100` are chosen as a scale of their relationship with eachother. 100 being best of friends, while -100 meaning the complete opposite.

Initially I will randomly assign 4 people from each group into their own group.

The next step is to calculate the energy state of the system. This is done by adding up the relationship value for each person in each group. Because each person will be compared twice (A->B and B->A), this result is divided by 2. 
Each person placed next to eachother in the group will also be calculated, and added to the total sum.

The next step is to define an initial temperature, for this I arbitrarily chose 100 degrees. The goal here is to swap everyone around randomly for each 'step' as the temperature cools off, and to measure the energy state of the system each time. If the swap resulted in a worse off energy, then a random number is generated up to the maximum current temperature value. If the energy difference is within this bounds, then the swap is allowed to proceed.

The idea of permitting a negative energy swap enables the system to potentially leave a local minimum (or maximum) and find a better result.

As the temperature trends to zero, the largest swaps have already taken place and movements have been reduced to local neighbours.

__Sample output__
```
$ ./annealer 
Group 0        | Group 1        
gerald         | florence       
hector         | ebeneezer      
claire         | alex           
bob            | dave           
Energy before: -13

 temperature: -5.25885e-05, energy: 400   
Group 0        | Group 1        
dave           | hector         
bob            | alex           
ebeneezer      | claire         
gerald         | florence       
Energy after: 500
```



## Building

Build with make
```
$ make
g++ -g build/main.o build/table.o -o annealer                                  
```


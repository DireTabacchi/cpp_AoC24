# Day 6: Evading the Guard

## Part 1

This one wasn't too hard. It was mainly about watching the guard and keeping track of every
position he had been to. Afterward, just count all the positions on the map and report that.

As for time, I was able to get this to run in somewhere between 1 to 4 milliseconds on full input.

## Part 2

This one was more challenging, while foresight made me think it would be just adding some more
tracking. My first thought was that if the guard visited a certain position from the same
direction at least 3 times, then there was definitely a loop. Why did I arbitrarily choose 3 times?
That answer even evades me.

My first major problem was figuring out why the `visited` field of a struct wasn't being updated.
After about an hour of looking and trying things, I realized that I forgot that C++ passes things
by *reference*; I had a range-`for` loop that was iterating over visited nodes to check if they
were visited, and incrementing the *copy* of the struct-field rather than the struct-field itself.
After that, things went pretty smoothly.

However, because of my naive first attempt, my solution was taking on average about *8 seconds*. I
believe the first change I made was removing the `patrol` string I was using to visually map the
path of the guard. This is also when I added the `unsigned long steps` global variable to keep
track of chiefly one thing: how many times was I tracking the guard's movement? You'll see in my
solution that I increment `steps` in the `while` loop in the `check_loop` function, and in the
`while` loop where I am initially watching the guard to track his path. Before all my changes, I
had logged around *19-20 million* steps being executed! So I went and looked at what I could do to
reduce this number, which I believed would significantly reduce my execution time.

So, the first thing I did was look through the `check_loop` function. The first thing I noticed was
I was calculating the guard's position every time from the map, which is a pretty long string to
look through. I made the guard's position a parameter of the function, and saw my execution time
decrease slightly, but there were still almost just as many `step`s being executed. What could I
do next?

The next thing I noticed was taking a lot of steps was just moving the guard forward. Before, every
iteration through the main `while` loop in `check_loop` was only moving the guard forward by 1
position if he were to move forward. So instead, I encapsulated each incrementation of the guard
moving forward, until he had reached another obstacle. My strategy for `check_loop` was that I
really only cared about positions where the guard *turns*, not whether he was moving forward.
Anyway, after these change, I saw an awesome improvement in execution steps and time: I went all
the way down to around 9.5 hundred thousand steps, and around a fraction of a second of execution
time!

The last thing that made any significance was, again, in moving the guard forward. I realized I
could check if he was at the edge of the map (and thus ready to leave the map) as I was moving him
forward. This brought my `steps` down to about 930,000, and my execution time to 300-400
milliseconds.

## Miscellanea

I made a small conditional for the small, test-input to be printed, to show the original starting
map, but not the larger real-input map. I figured that it was so big, it would be annoying to
scroll up and down between the maps to see the differences. Also, while I have the map of possible
obstructions being printed, even for the large map, I only included the creation of that map in the
execution time. I believe that including the reporting of results into the execution time is quite
silly, and really wouldn't impact the total time taken to complete the calculation anyways.

One thing that I am still unsure of is whether to include reading of the input file in the
execution time or not. On one hand, just trying to read the input into a string feels like it
shouldn't be included, as some languages like Zig allow for the embedding of files into the binary
itself (as to whether C++ can do that, I am unaware). On the other hand, if the input is already in
a format that it would be trivial to read the data directly from the file (as with C++ file streams,
see some of my previous solutions), I feel that that *must* be included in the execution time.

Being the 6th day of the Advent of Code, and my 5th solution (sorry day 5! I thought this was you!),
I want to be able to complete this entire event. Solving these problems is entirely satisfying,
especially with this challenge that had me pump a fist in the air when I submitted my result from
my original, slow calculation and it was accepted. The optimization was just the cherry on top,
and so delicious to experience.


# ProblemSet9

<!-- after a few hours of tutoring with zach I was able to better understand problemset 7. I then had him help me with this problem set and was able to get through 9.1.

I've spent hours and HOURS trying to figure out 9.2. the bitcrusher code you gave us deals with short type value. When i try to change that value, this line of code:

     return (input & (-1 << (16-keepBits)));

doesn't work. 
So instead I tried changing the inBuffer[n] (float value) to short value so it'd work with the function you gave us and the code didn't like that either. 
As far as I know, if the value issue wasn't there, this code would work. I don't feel like I have the nessesary knowledge to complete these so I'm turning in what I have.
  -->

9.2 just ended up being an issue of typecasting like I had thought. I just needed to refresh myself on shorts, ints, short ints, and typecasting.

9.3 i had to have Zach show how the MIN and MAX functions within the BassBoosta you gave to us. the actual function declaration didn't work until we replaced saturation with MIN(MAX)

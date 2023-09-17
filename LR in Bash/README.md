### Description
---
This is a simple yet working example of one-variable linear regression model, coded in Bash.
To train the model, run the `fit.sh` command. This will return the slope and intercept of the line.
To predict a value, run the `predict.sh` command.
For simplicity, `X_train` is simply the auto-incremented index variable starting from 1. So, you only need one column `y_train` to get started.

### Usage note
---
For best experience you can pipe your input data with these two commands, so that the output of `fit.sh` is the input for `predict.sh`, like this

![](lr2.gif)

Now, this is a tiny ML pipeline! I wonder if Bash creators had such an idea in their minds back then :-). This nice animation was created with the help of [congif](https://github.com/lecram/congif) and [ezgif](https://ezgif.com/).

### Implementation note
---
Since Bash does not support floats, both the input data and the model's parameters are integer values, which may lead to inaccurate results.

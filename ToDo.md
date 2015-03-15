# To do #

Several parts of Cornucopia could use improvement:
  * The corner detection algorithm could be better.  Ideally, there'd be a version that can take timing information into account, leading to better accuracy.
  * The oversketching can be improved: right now it can still produce very short segments near transition points
  * The code could be better documented--not all algorithms are clear
  * Investigate using the oversketching functionality to get an "online" mode, where the curve is fit in pieces as the user is drawing, for instant feedback and better performance on huge curves at the cost of some fairness/fit quality.
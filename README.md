**Simple visualizer for School 42 Philosophers project, written in C + Raylib.**

![Philosophers Visualizer](img/img1.png)

To use, you might need to add fflush(stdout) to your philosophers output to flush the buffer after each line.

**To launch:**
1. Clone this repo into your philo project: philo/visualizer
1. Compile the visualizer: make -C visualizer
1. ./philo <10> <800> <200> <200> | ./visualizer/visualizer

It will pick up the number of philosophers automatically on the go.

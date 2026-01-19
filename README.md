**Simple visualizer for School 42 Philosophers project, written in C + Raylib.**

![Philosophers Visualizer](img/img1.png)

## Compilation and launch 🚀
**1. Clone this repo into your philo project: philo/visualizer (or any other folder name):**
```bash
git clone https://github.com/dmitrijslasko/42_philosophers_visualizer visualizer
```
**2. Compile the visualizer:**
```bash
make -C visualizer
```
**3. Launch:**
```bash
./philo <10> <800> <200> <200> | ./visualizer/visualizer
```
The output of your philosophers' program will go into the visualizer through a pipe.
It will pick up the number of philosophers automatically on the go.

**Ensure your output matches the format specified in the subject PDF!**
**You might need to add fflush(stdout) to your philosophers output to flush the buffer after each line.**

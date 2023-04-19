Input:
  Movement: WASD
  Interacting: Left mouse button

If pi build doesn't have a good connection look at the project settings > Debugging > remotedebug machine
and reallocate your connection.

This file is a fallback in case the remote pre-build event does not successfully download the required libs:
Use team24libs.sh on the pi to get the required libs
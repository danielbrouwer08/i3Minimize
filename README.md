# i3Minimize
Application that enables minimizing in the i3 window manager

I really missed the possibility for i3 to minimize windows therefore I made this simple GTK3 application that enables/simulates minimizing by using the scratchpad.

# Dependencies
Make sure you have installed the following packages before installing i3Minimize:
	tmux (used for sending commands to the i3Minimize application)
	xdotool (used for getting the XID of the application to be minimized)
	i3 (obviously)
	gtk3

# Installation
To compile and install just run the install script.

	./install (from the root directory)

Be sure that the script is enabled for execution with the following command:

	sudo chmod 755 ./install (from the root directoy)

After installation add the following line to the i3 config file (~/.i3/config):

	exec --no-startup-id i3m

To enable auto-starting of i3Minimize

Furthermore replace this line:
	
	bindsym \$mod+Shift+minus move scratchpad

with:
	
	$mod+Shift+minus exec sendInput \$(getFocussed)

To disable the default scratchpad behaviour and enable the new minimize beheviour. Of course it is also possible to use another shortcut key combination.

# Removal
The application can be removed using the remove script.

	./remove (from the root directory)

Again make sure that the script in enabled for execution.

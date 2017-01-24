# i3Minimize
Application that enables minimizing in the i3 window manager

I really missed the possibility for i3 to minimize windows therefore I made this simple GTK3 application that enables/simulates minimizing by using the scratchpad.

# Example images
i3Minimize with Firefox minimzed and two terminals open:
![alt tag](https://github.com/danielbrouwer08/i3Minimize/raw/master/images/i3minimize_example1.png)

i3Minimize with both Firefox and one of the terminals minimized
![alt tag](https://github.com/danielbrouwer08/i3Minimize/raw/master/images/i3minimize_example2.png)

# Dependencies
Make sure you have installed the following packages before installing i3Minimize:
	
	tmux (used for sending commands to the i3Minimize application)
	xdotool (used for getting the XID of the application to be minimized)
	i3 (obviously)
	gtk3
	xterm (should not be necessary in the future)

# Installation
First clone this repository using the following command:

	git clone https://github.com/danielbrouwer08/i3Minimize.git

Enter the directory that has been created:
	
	cd ./i3Minimize

To compile and install just run the install script.

	./install (from the root directory)

If it does not work, be sure that the script is enabled for execution with the following command:

	sudo chmod 755 ./install (from the root directoy)

After installation add the following line to the i3 config file (~/.i3/config):

	exec --no-startup-id i3m

To enable auto-starting of i3Minimize

Furthermore replace this line:
	
	bindsym \$mod+Shift+minus move scratchpad

with:
	
	$mod+Shift+minus exec sendInput $(getFocussed)

To disable the default scratchpad behaviour and enable the new minimize beheviour. Of course it is also possible to use another shortcut key combination.

# Launching i3Minimize
i3Minimize can be launched using the following command:

	i3m

# Usage
An active window can be minimized by using the shorcut defined in the i3 config file. If you have followed the installation steps the shortcut should be $mod+Shift+minus. The name and XID of the window will now appear in the i3Minimize bar. To retrieve the window just click on the corresponding button.

# Future plans
* Keyboard shortcuts to maximize a specific window after it has been minimized.
* Renaming of the labels.
* Remove xterm dependency by using the default installed terminal.

# Removal
The application can be removed using the remove script.

	./remove (from the root directory)

Again make sure that the script in enabled for execution.

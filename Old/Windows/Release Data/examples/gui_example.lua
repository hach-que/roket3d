-- Roket3D GUI Example
-- Public Alpha 0.1 Compatable

-- First create a device.  This is the graphical window you see.
mydevice = Core.RDevice();

-- Store the GUI Environment in a variable.
myenv = mydevice:getGUIEnvironment();

-- Store the Scene Manager in a variable.
myscene = mydevice:getSceneManager();

-- Store the Video Driver in a variable.
myvdriver = mydevice:getVideoDriver();

-- Create a button from 0,0 to 320,32 in the environment myenv.  Set it's ID to 2.
mybutton = myenv:addButton({0,0,320,32});

-- Set the buttons name
mybutton:setName("btnHelloWorld");

-- Show the buttons name
print("My button's name is : " .. mybutton:getName());

-- Display the buttons current ID.
print("My Button's ID is : " .. mybutton:getID());

-- Change the buttons ID to 4.
mybutton:setID(4);

-- Display the buttons ID again.
print("My Button's ID is now : " .. mybutton:getID());

-- Change the text of our button.
mybutton:setText("The Hello World Button!");

-- Create our editbox from 8,40 to 640-8,480-8 in the environment myenv.  Set it's name to "txtEditBox"
myeditbox = myenv:addEditBox({8,40,640-8,480-8},nil,"txtEditBox");

-- Setup the editbox
myeditbox:setText("Type something here, then press the button.");
myeditbox:setMax(100);
myeditbox:setWordWrap(true);

-- Create some variables.
x=0;
lastFPS=0;

-- Create an event receiver
eventreceiver = Core.RoketEventReceiver();

-- Define the event receiver function
function handleEvent(ud1, ud2, ud3, ud4)
	if (ud2=="btnHelloWorld") then
		if (ud1=="Clicked") then
			mybutton:setText(myeditbox:getText());
			myeditbox:setText("The magic of Roket3D!");
		end
	end
end

-- Set the event receiver's function
eventreceiver:setGUIEventCapturer("handleEvent");

-- Assign the event receiver to the device
mydevice:setEventReceiver(eventreceiver);

-- Set the window caption.
mydevice:setWindowCaption("Hello World!");

-- While device-is-running loop.  Unless you want your window to stay open for 1 CPU cycle, you have to do this.
while (mydevice:isRunning()) do
		
	-- If x is less than 300
	if (x<300) then
		
		-- Increase x by 1
		x=x+1;
		
		-- Update the elements position
		mybutton:setPos({x,0,x+320,32});
	end
		
	-- Begin drawing the scene
	myvdriver:beginScene(true,true,{130,130,130});
	
	-- Draw the scene
	myscene:drawAll();
	
	-- Draw the GUI Environment
	myenv:drawAll();
	
	-- Finish drawing the scene
	myvdriver:endScene();
	
	-- Update the window title with the FPS, only if it isn't the same as the previous FPS.
	fps = myvdriver:getFPS()
	if (lastFPS~=fps) then
		mydevice:setWindowCaption("Hello World! FPS : " .. fps);
		lastFPS=fps;
	end
end

-- Close the device.  This closes the graphical window, and all graphical elements.
--mydevice:close();

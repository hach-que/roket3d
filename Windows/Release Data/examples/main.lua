-- Roket3D Main Menu
-- Works with Roket3D Alpha 0.4

immediateQuit=false;
dev = Core.RDevice(Core.Vector2D(640,480),32);

--font = dev:getGUIEnvironment():getFont("examples/fontlucida.png");
--dev:getGUIEnvironment():getSkin():setFont(font);

btn_GUIExample = dev:getGUIEnvironment():addButton(
				{488-80,24,616,40},
				nil,
				"btn_GUIExample");

btn_SceneExample = dev:getGUIEnvironment():addButton(
				{488-80,48,616,64},
				nil,
				"btn_SceneExample");
				
txt_DescriptionArea = dev:getGUIEnvironment():addStaticText(
				{24,24,464-80,456},
				nil,
				"txt_DescriptionArea");

btn_GUIExample:setText("GUI Example");
btn_SceneExample:setText("Scene (3D) Example");
txt_DescriptionArea:setText("Move your mouse over a button to see a description of the example.");
txt_DescriptionArea:setWordWrap(true);

thread = Ext.RThread();
shouldQuit=false;

function handleMenuGUIEvent(ud1, ud2, ud3, ud4)
	if (ud2=="btn_GUIExample") then
		if (ud1=="Clicked") then
			-- Now start the GUI Example.
			shouldQuit=true;
			thread:changeFile("examples/gui_example.lua");
			thread:start();
		end
		if (ud1=="MouseOver") then
			txt_DescriptionArea:setText("This example shows the basics of GUI elements.  It only has a button and a text area.  It also shows basic event handling.");
		end
	end
	if (ud2=="btn_SceneExample") then
		if (ud1=="Clicked") then
			-- Now start the GUI Example.
			shouldQuit=true;
			thread:changeFile("examples/scene_example.lua");
			thread:start();
		end
		if (ud1=="MouseOver") then
			txt_DescriptionArea:setText("This example shows how to load a model into the scene environment as well as creating a FPS camera.");
		end
	end
	if (ud1=="MouseOut") then
		txt_DescriptionArea:setText("Move your mouse over a button to see a description of the example.");
	end
end
maineventreceiver = Core.RoketEventReceiver();
maineventreceiver:setGUIEventCapturer("handleMenuGUIEvent");
dev:setEventReceiver(maineventreceiver);
lastFPS=0;
dev:setWindowCaption("Roket3D Examples Main Menu");

myvdriver = dev:getVideoDriver();

while (dev:isRunning() and shouldQuit==false) do
	myvdriver:beginScene(true,true,{130,130,130});
	dev:getGUIEnvironment():drawAll();
	myvdriver:endScene();
	fps = myvdriver:getFPS()
	if (lastFPS~=fps) then
		dev:setWindowCaption("Roket3D Examples Main Menu FPS : " .. fps);
		lastFPS=fps;
	end
end

dev:close();
		
-- Wait till all the examples have finished before killing everything...
-- If we didn't do this, then the main program would terminate all threads
-- thus closing the examples.  It is generally not safe to terminate threads
-- using thread:stop() especially if the thread works with files or is
-- important to your game.  You should also try and NOT lose the variable to
-- the thread like so:
--
-- thread = Ext.RThread("path/to/file.lua")
-- thread:start();
-- thread = 0;
--
-- As it is impossible to wait till it is finished, though the thread will
-- continue running, as long as the main script does not exit.
thread:waitTillFinished()
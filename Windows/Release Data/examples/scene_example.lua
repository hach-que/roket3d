-- Roket3D Scene Example
-- Public Alpha 0.3 Compatable

-- This only has comments on functions that have not been explained in gui_example.lua

mydevice = Core.RDevice();
myscene = mydevice:getSceneManager();
myenv = mydevice:getGUIEnvironment();
myvdriver = mydevice:getVideoDriver();
mydevice:setWindowCaption("Scene Example (Hit Alt-F4 to exit)");
myeditbox = myenv:addStaticText(myenv,{8,8,320,24},nil,"txtEditBox");
myeditbox:setText("Scene Example (Hit Alt-F4 to exit)");

mesh = myscene:getMesh("examples/sydney.md2");
node = myscene:addMeshSceneNode(mesh);
tex = myvdriver:getTexture("examples/sydney.bmp");
camera = myscene:addCameraSceneNodeFPS(100,0);
camera:setSourcePosition({50,50,50});
camera:setDestinationPosition({0,0,0});
node:applyTexture(tex,0);
node:applyFlag("LIGHTING",false);

--a=300;
--b=0;

while (mydevice:isRunning()) do
	--if (b==10) then
	--	a=a-1;
	--	camera:setSourcePosition({-a,-a,-a});
	--	b=0;
	--else
	--	b=b+1;
	--end
	myvdriver:beginScene(true,true,{130,130,130});
	myscene:drawAll();
	myenv:drawAll();
	myvdriver:endScene();

	fps = myvdriver:getFPS();
	if (lastFPS~=fps) then
		mydevice:setWindowCaption("Scene Example (Hit Alt-F4 to exit) FPS : " .. fps);
		lastFPS=fps;
	end
end
mydevice:close();

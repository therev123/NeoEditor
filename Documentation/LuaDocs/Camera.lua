changeCurrentCamera(object);
getCurrentCamera();
getCameraClearColor(object);
getCameraFov(object);
getCameraNear(object);
getCameraFar(object);
getCameraFogDistance(object);
isCameraOrtho(object);
isCameraFogEnabled(object);
setCameraClearColor(object, {r, g, b});
setCameraFov(object, fov);
setCameraNear(object, near);
setCameraFar(object, far);
setCameraFogDistance(object, fogDistance);
enableCameraOrtho(object, ortho);
enableCameraFog(object, fog);
enableCameraLayer(object, scene);
disableCameraLayer(object);
enableRenderToTexture(object, "textureName", renderWith, renderHeight);
disableRenderToTexture(object);
getProjectedPoint(object, point);
getUnProjectedPoint(object, point);
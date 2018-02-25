#pragma once
#include <memory>
#include "IModelListener.h"
class IModelObservable
{
public:
	virtual ~IModelObservable()
	{
	}

	virtual void subscribe(std::shared_ptr<IModelListener> listener) = 0;



	/*
	void subscribe(ModelListener listener);

    void handleOpenSceneFile(SceneFileInfo sceneFileInfo);

    void handleUpdateMainImage();

    void handleMoveCamera(double xAngle, double yAngle, double zAngle);

    void handleZoomCamera(double delta);

    void handleMoveCameraToRight();

    void handleMoveCameraToLeft();

    void handleMoveCameraToUp();

    void handleMoveCameraToDown();

    void handleChangeZnWithWeel(double ratio);

    void handleChangeProperties(Properties properties);

    void handleGetProperties();

    void handlePushOnInitButton();

    void handleRenderScene();

    void handleShowWireframe();

    void handleOpenRenderFile(RenderInfo fileInfo);

    void handleSaveImage(File file) throws IOException;

    void handleOpenSceneFileWithRender(SceneFileInfo sceneFileInfo, RenderInfo fileInfo);

    void handleNewPropertiesFromDialog(DialogProperties dialogProperties);

    void handleSaveRenderFile(File file) throws IOException;

    void handleNewHeight(int height);
	 */

};


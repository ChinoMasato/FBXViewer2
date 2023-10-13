# include <Siv3D.hpp> // OpenSiv3D v0.6.11
#include <fbxsdk.h>
const String GetNodeAttributeName(FbxNodeAttribute::EType attribute)
{
	static int skeleton_counter = 0;
	switch (attribute)
	{
	case fbxsdk::FbxNodeAttribute::eUnknown:
		return U"eUnknown";
		break;
	case fbxsdk::FbxNodeAttribute::eNull:
		return U"eNull";
		break;
	case fbxsdk::FbxNodeAttribute::eMarker:
		return U"eMarker";
		break;
	case fbxsdk::FbxNodeAttribute::eSkeleton:
		skeleton_counter++;
		//Print << skeleton_counter;
		return U"eSkeleton";
		break;
	case fbxsdk::FbxNodeAttribute::eMesh:
		return U"eMesh";
		break;
	case fbxsdk::FbxNodeAttribute::eNurbs:
		return U"eNurbs";
		break;
	case fbxsdk::FbxNodeAttribute::ePatch:
		return U"ePatch";
		break;
	case fbxsdk::FbxNodeAttribute::eCamera:
		return U"eCamera";
		break;
	case fbxsdk::FbxNodeAttribute::eCameraStereo:
		return U"eCameraStereo";
		break;
	case fbxsdk::FbxNodeAttribute::eCameraSwitcher:
		return U"eCameraSwitcher";
		break;
	case fbxsdk::FbxNodeAttribute::eLight:
		return U"eLight";
		break;
	case fbxsdk::FbxNodeAttribute::eOpticalReference:
		return U"eOpticalReference";
		break;
	case fbxsdk::FbxNodeAttribute::eOpticalMarker:
		return U"eOpticalMarker";
		break;
	case fbxsdk::FbxNodeAttribute::eNurbsCurve:
		return U"eNurbsCurve";
		break;
	case fbxsdk::FbxNodeAttribute::eTrimNurbsSurface:
		return U"eTrimNurbsSurface";
		break;
	case fbxsdk::FbxNodeAttribute::eBoundary:
		return U"eBoundary";
		break;
	case fbxsdk::FbxNodeAttribute::eNurbsSurface:
		return U"eNurbsSurface";
		break;
	case fbxsdk::FbxNodeAttribute::eShape:
		return U"eShape";
		break;
	case fbxsdk::FbxNodeAttribute::eLODGroup:
		return U"eLODGroup";
		break;
	case fbxsdk::FbxNodeAttribute::eSubDiv:
		return U"eSubDiv";
		break;
	case fbxsdk::FbxNodeAttribute::eCachedEffect:
		return U"eCachedEffect";
		break;
	case fbxsdk::FbxNodeAttribute::eLine:
		return U"eLine";
		break;
	}

	return U"";
}
void PrintNode(FbxNode* node, int hierarchy, TextWriter writer)
{
	String OutputText = U"";
	if (node == nullptr)
	{
		return;
	}
	const char* name = node->GetName();

	for (int i = 0; i < hierarchy; i++)
	{
		OutputText+= U"\t";
	}

	OutputText += U"【"+Unicode::Widen(name)+U"】\n";

	for (int i = 0; i < node->GetNodeAttributeCount(); i++)
	{
		for (int j = 0; j < hierarchy; j++)
		{
			OutputText +=  U"\t";
		}
		writer << OutputText + U"Attribute->" << GetNodeAttributeName(node->GetNodeAttributeByIndex(i)->GetAttributeType());
	}

	for (int i = 0; i < node->GetChildCount(); i++)
	{
		PrintNode(node->GetChild(i), hierarchy + 1, writer);
	}
}
void Main()
{
	// ファイルオープンダイアログを使ってファイルを選択
	Optional<FilePath> path;


	while (System::Update())
	{
		if (SimpleGUI::Button(U"Open .fbx file", Vec2{ 20,180 }))
		{
			// 独自の拡張子
			path = Dialog::OpenFile({ { U"FBX file",{ U"fbx" } } });

			if (path) {
				Window::Resize(1200, 800);
				// FbxManager作成
				FbxManager* fbx_manager = FbxManager::Create();
				if (fbx_manager == nullptr)
				{
					// 作成失敗
					return;
				}

				// FbxImporter作成
				FbxImporter* fbx_importer = FbxImporter::Create(fbx_manager, "ImportTest");
				if (fbx_importer == nullptr)
				{
					fbx_manager->Destroy();

					// 作成失敗
					return;
				}
				// FbxScene作成
				FbxScene* fbx_scene = FbxScene::Create(fbx_manager, "SceneTest");
				if (fbx_scene == nullptr)
				{
					fbx_importer->Destroy();
					fbx_manager->Destroy();

					// 作成失敗
					return;
				}
				// ファイルを初期化する

				if (fbx_importer->Initialize((*path).narrow().c_str()) == false)
				{
					fbx_importer->Destroy();
					fbx_scene->Destroy();
					fbx_manager->Destroy();

					// 初期化失敗
					return;
				}
				// インポート
				if (fbx_importer->Import(fbx_scene) == false)
				{
					fbx_importer->Destroy();
					fbx_scene->Destroy();
					fbx_manager->Destroy();

					// インポート失敗
					return;
				}

				FbxNode* root_node = fbx_scene->GetRootNode();

				int count = fbx_scene->GetNodeCount();

				TextWriter writer(FileSystem::FileName(*path)+U".txt");
				PrintNode(root_node, 0, writer);

				if (fbx_importer != nullptr)
				{
					fbx_importer->Destroy();
				}

				if (fbx_scene != nullptr)
				{
					fbx_scene->Destroy();
				}

				if (fbx_manager != nullptr)
				{
					fbx_manager->Destroy();
				}
			}

		}
	}
}


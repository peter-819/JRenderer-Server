#define _CRT_SECURE_NO_WARNINGS
#include <JEngine.h>
#include "GL/glew.h"
#include <fstream>
#include <jpeglib.h>
#include <stdio.h>
#include <setjmp.h>
#include <queue>
#define JPEG_LIB_VERSION 62

#define JAPI extern "C" __declspec(dllexport) 

JSAMPLE* image_buffer;   /* Points to large array of R,G,B-order data */
int image_height;        /* Number of rows in image */
int image_width;         /* Number of columns in image */

void write_JPEG_file(char* filename, int quality)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	///* More stuff */
	FILE* outfile;                /* target file */
	JSAMPROW row_pointer[1];      /* pointer to JSAMPLE row[s] */
	int row_stride;               /* physical row width in image buffer */

	cinfo.err = jpeg_std_error(&jerr);
	///* Now we can initialize the JPEG compression object. */
	jpeg_create_compress(&cinfo);

	if ((outfile = fopen(filename, "wb")) == NULL) {
		fprintf(stderr, "can't open %s\n", filename);
		exit(1);
	}
	jpeg_stdio_dest(&cinfo, outfile);

	cinfo.image_width = image_width;      /* image width and height, in pixels */
	cinfo.image_height = image_height;
	cinfo.input_components = 3;           /* # of color components per pixel */
	cinfo.in_color_space = JCS_RGB;       /* colorspace of input image */

	jpeg_set_defaults(&cinfo);

	jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

	jpeg_start_compress(&cinfo, TRUE);

	row_stride = image_width * 3; /* JSAMPLEs per row in image_buffer */

	while (cinfo.next_scanline < cinfo.image_height) {
		JE_CORE_INFO("{0}", cinfo.next_scanline);
		row_pointer[0] = &image_buffer[cinfo.next_scanline * row_stride];
		(void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	///* Step 6: Finish compression */

	jpeg_finish_compress(&cinfo);
	///* After finish_compress, we can close the output file. */
	fclose(outfile);

	///* Step 7: release JPEG compression object */

	///* This is an important step since it will release a good deal of memory. */
	jpeg_destroy_compress(&cinfo);

	///* And we're done! */
}

void write_JPEG_Buffer(int quality, unsigned long* jpegSize, unsigned char** jpegBuffer) {
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	JSAMPROW row_pointer[1];
	int row_stride;

	cinfo.err = jpeg_std_error(&jerr);

	jpeg_create_compress(&cinfo);
	jpeg_mem_dest(&cinfo, jpegBuffer, jpegSize);

	cinfo.image_width = image_width;
	cinfo.image_height = image_height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE);

	jpeg_start_compress(&cinfo, TRUE);

	row_stride = image_width * 3;
	while (cinfo.next_scanline < cinfo.image_height) {
		row_pointer[0] = &image_buffer[cinfo.next_scanline * row_stride];
		jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);

}

typedef void (*SendDataToClient)(unsigned char* ptr, unsigned long size);

std::vector<std::uint8_t> data;
unsigned char* buffer = nullptr;
SendDataToClient sendData = nullptr;
int AppWidth, AppHeight;
unsigned char* SendJPEGBuffer() {
	if (buffer) {
		free(buffer);
		buffer = nullptr;
	}
	if (data.size() < AppWidth * AppHeight * 3) {
		data.resize(AppWidth * AppHeight * 3);
	}
	glReadBuffer(GL_BACK);
	glReadPixels(0, 0, AppWidth, AppHeight, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);
	//std::ofstream fout("testOutput.ppm");
	//fout << "P3" << std::endl;
	//fout << AppWidth << " " << AppHeight << std::endl;
	//fout << 255 << std::endl;
	//for (int i = 0; i < AppWidth * AppHeight;i++) {
	//	fout << (int)data[i * 4 + 0] << " " << (int)data[i * 4 + 1] << " " << (int)data[i * 4 + 2] << " ";
	//}
	//fout.close();
	image_buffer = &data[0];
	image_width = AppWidth;
	image_height = AppHeight;
	//write_JPEG_file("output.jpg", 80);
	unsigned long size = 0;
	write_JPEG_Buffer(90, &size, &buffer);
	sendData(buffer, size);
	return buffer;
}


enum ShapeID {
	Teapot, Cube, Plane, FileShape
};
class ExampleLayer : public JEngine::Layer {
public:
	ExampleLayer() :Layer("Example") {}

	void OnImGuiRender() override {
		ImGui::SetNextWindowSizeConstraints(ImVec2(500, 200), ImVec2(1000, 1000));
		ImGui::Begin("Light Setting");

		if (ImGui::Button("Add Light")) {
			m_Light[LightNum].setLight({ -5.0f, 4.0f, 5.0f }, 0.2f, 0.6f, 6.0f, 20.0f);
			LightNum++;
		}
		char str[100];
		for (int i = 0; i < LightNum; i++) {
			ImGui::Text("Light %d:", i);

			sprintf(str, "Light[%d].IsAbled", i);
			if (ImGui::Button(str)) m_Light[i].isAbled = !m_Light[i].isAbled;

			sprintf(str, "Light[%d].AmbientLight", i);
			ImGui::SliderFloat(str, &m_Light[i].AmbientLightScalar, 0.0f, 1.0f);

			sprintf(str, "Light[%d].DiffuseLight", i);
			ImGui::SliderFloat(str, &m_Light[i].DiffuseLightScalar, 0.0f, 1.0f);

			sprintf(str, "Light[%d].SpecularLight Radius", i);
			ImGui::SliderFloat(str, &m_Light[i].SpecularLightRadius, 0.0f, 30.0f);

			sprintf(str, "Light[%d].SpecularLight Pow", i);
			ImGui::SliderFloat(str, &m_Light[i].SpecularLightPow, 0.0f, 30.0f);

			glm::vec3 p = m_Light[i].getLightPosition();
			float v[3] = { p.x,p.y,p.z };
			sprintf(str, "Light[%d].Light Position", i);
			ImGui::DragFloat3(str, v, 0.2f);
			m_Light[i].setLightPosition(v[0], v[1], v[2]);
		}

		ImGui::End();
	}

	void OnAttach() override {
		m_shaders.Load("asset\\ColorShader.glsl");
		m_shaders.Load("asset\\LightCubeShader.glsl");

		m_Light[0].setLight({ -5.0f, 4.0f, 5.0f }, 0.2f, 0.6f, 6.0f, 20.0f);
		m_Plane = JEngine::MeshGenerator::makePlane(50);
		m_LightCube = JEngine::MeshGenerator::makeCube();
		//m_Mesh = JEngine::MeshGenerator::LoadBunny(".\\asset\\bunny.ply");
		//m_Light.BindShader(m_shader);

	}

	void setUniforms(JEngine::Ref<JEngine::Shader> shader) {
		shader->Bind();
		char str[100];
		for (int i = 0; i < LightNum; i++) {
			sprintf(str, "Light[%d].Position", i);
			shader->SetFloat3(str, m_Light[i].getLightPosition());
			sprintf(str, "Light[%d].Ambient", i);
			shader->SetFloat(str, m_Light[i].AmbientLightScalar);
			sprintf(str, "Light[%d].Diffuse", i);
			shader->SetFloat(str, m_Light[i].DiffuseLightScalar);
			sprintf(str, "Light[%d].Radius", i);
			shader->SetFloat(str, m_Light[i].SpecularLightRadius);
			sprintf(str, "Light[%d].Pow", i);
			shader->SetFloat(str, m_Light[i].SpecularLightPow);
			sprintf(str, "Light[%d].isAbled", i);
			shader->SetBool(str, m_Light[i].isAbled);
		}
	}

	void OnUpdate(float dt) override {
		QueueLock.lock();
		while (!CommandQueue.empty()) {
			auto command = CommandQueue.front();
			CommandQueue.pop();
			glm::mat4 transform = glm::translate(glm::vec3(command.x,command.y,command.z))
				* glm::scale(glm::vec3(command.scale))
				* glm::rotate(-glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

			switch (command.shape) {
			case 0:
				m_Meshes.push_back(JEngine::MeshGenerator::makeCube());
				m_Transforms.push_back(transform);
				break;
			case 1:
				break;
			case 2:
				//m_Meshes.push_back(JEngine::MeshGenerator::LoadBunny(".\\asset\\bunny.ply"));
				//m_Transforms.push_back(glm::mat4(1.0f));
				break;
			case 3:
				m_Meshes.push_back(std::make_shared<JEngine::Mesh>(".\\asset\\Donut2.fbx"));
				m_Transforms.push_back(transform);
				break;
			case 4:
				m_Light[LightNum].setLight({ -5.0f, 4.0f, 5.0f }, 0.2f, 0.6f, 6.0f, 20.0f);
				LightNum++;
			}
		}
		QueueLock.unlock();
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		m_CameraController.OnUpdate(dt);
		glm::mat4 transform;
		JEngine::RenderCommand::Clear();
		JEngine::RenderCommand::SetClearColor({ 0.1f,0.1f,0.1f,1.0f });

		JEngine::Ref<JEngine::Shader> shader = m_shaders.Get("ColorShader");

		setUniforms(shader);

		JEngine::Renderer::BeginScene(m_CameraController.GetCamera());
		transform = glm::translate(glm::vec3(0.0f, 0.5f, 0.0f)) * glm::rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		for (int i = 0; i < LightNum; i++) {
			transform = glm::translate(m_Light[i].getLightPosition()) * glm::scale(glm::vec3(0.3f, 0.3f, 0.3f));
			m_LightCube->render(transform, m_shaders.Get("LightCubeShader"));
		}

		transform = glm::translate(glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::scale(glm::vec3(10.0f, 10.0f, 10.0f))
			* glm::rotate(-glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		
		//m_Mesh.render(transform, m_shaders.Get("ColorShader"));
		for (int i = 0; i < m_Meshes.size(); i++) {
			m_Meshes[i]->render(m_Transforms[i], m_shaders.Get("ColorShader"));
		}
		m_Plane->render(glm::mat4(1.0f), m_shaders.Get("ColorShader"));
		SendJPEGBuffer();
		JEngine::Renderer::EndScene();
	}

	void OnEvent(JEngine::Event& event) override {
		m_CameraController.OnEvent(event);
	}
	public:
		struct MeshCommand {
			int shape;
			double scale;
			double x, y, z;
		};
public:
	JEngine::Ref<JEngine::Mesh> m_LightCube, m_Plane;
	std::vector<JEngine::Ref<JEngine::Mesh>> m_Meshes;
	std::queue<MeshCommand> CommandQueue;
	std::mutex QueueLock;
	std::vector<glm::mat4> m_Transforms;
	JEngine::ShaderLibrary m_shaders;
	JEngine::ProjectiveCameraController m_CameraController;
	JEngine::Light m_Light[10];
	int LightNum = 1;
};

class sandbox : public JEngine::Application {
public:
	sandbox(JEngine::AppProps props) : JEngine::Application(props) {
		PushLayer(new ExampleLayer());
	}
	~sandbox() {}
};

JEngine::Application* JEngine::createApplication(JEngine::AppProps props) {
	return new sandbox(props);
}

JAPI JEngine::Application* CreateJEngine(HWND hwnd, int width,int height) {
	JEngine::Log::Init();
	JE_CORE_WARN("Initialized Log!");
	JEngine::AppProps props{};
	AppWidth = width;
	AppHeight = height;
	props.height = height;
	props.width = width;
	props.hwnd = hwnd;
	auto p = JEngine::createApplication(props);
	return p;
}

JAPI void JEngineRun(JEngine::Application* app) {
	app->AppRun();
}

JAPI void SetSendDataCallback(SendDataToClient call) {
	sendData = call;
}

JAPI void OnCreate(JEngine::Application* app, int shape,double scale,double x,double y,double z) {
	auto* layer = dynamic_cast<ExampleLayer*>(app->GetMainRenderLayer());
	layer->QueueLock.lock();
	layer->CommandQueue.push(ExampleLayer::MeshCommand{ shape,scale,x,y,z });
	layer->QueueLock.unlock();
}

JAPI void OnCreateLight(JEngine::Application* app, double als, double dls, double slr, double slp, double x, double y, double z) {
	auto* layer = dynamic_cast<ExampleLayer*>(app->GetMainRenderLayer());
	layer->m_Light[layer->LightNum++].setLight(glm::vec3(x, y, z), als, dls, slr, slp);
}
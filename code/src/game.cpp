
#include <rawrbox/render/cameras/perspective.hpp>
#include <rawrbox/render/models/utils/mesh.hpp>
#include <rawrbox/render/resources/texture.hpp>
#include <rawrbox/resources/manager.hpp>

#include <my-game/game.hpp>

namespace my_game {
	void Game::setupGLFW() {
#if defined(_DEBUG) && defined(RAWRBOX_SUPPORT_DX12)
		auto* window = rawrbox::Window::createWindow(Diligent::RENDER_DEVICE_TYPE_D3D12); // DX12 is faster on DEBUG than Vulkan, due to vulkan having extra check steps to prevent you from doing bad things
#else
		auto window = rawrbox::Window::createWindow();
#endif

		window->setMonitor(-1);
		window->setTitle("My game");
#ifdef _DEBUG
		window->init(1024, 768, rawrbox::WindowFlags::Window::WINDOWED);
#else
		window->init(-1, -1, rawrbox::WindowFlags::Window::BORDERLESS);
#endif

		window->onWindowClose += [this](auto& /*w*/) { this->shutdown(); };
	}

	void Game::init() {
		auto* window = rawrbox::Window::getWindow();

		// Setup renderer
		auto* render = window->createRenderer();
		render->onIntroCompleted = [this]() { this->loadContent(); };
		render->setDrawCall([this](const rawrbox::DrawPass& pass) {
			if (pass == rawrbox::DrawPass::PASS_OVERLAY) {
				this->drawOverlay();
			} else if (pass == rawrbox::DrawPass::PASS_OPAQUE) {
				this->drawWorld();
			}
		});
		// ---------------

		// Setup camera --
		auto* cam = render->setupCamera<rawrbox::CameraPerspective>(render->getSize());
		cam->setPos({0.F, 5.F, -5.F});
		cam->setAngle({0.F, rawrbox::MathUtils::toRad(-45), 0.F, 0.F});
		// ---------------

		// Add loaders
		rawrbox::RESOURCES::addLoader<rawrbox::TextureLoader>();
		//  --------------

		render->init();
	}

	void Game::loadContent() {
		std::vector<std::pair<std::string, uint32_t>> initialContentFiles = {
		    {"./assets/textures/rawrbox.png", 0},
		};

		rawrbox::RESOURCES::loadListAsync(initialContentFiles, [this]() {
			rawrbox::runOnRenderThread([this]() {
				this->contentLoaded();
			});
		});
	}

	void Game::contentLoaded() {
		if (this->_ready) return;

		// Textures ---
		this->_texture = rawrbox::RESOURCES::getFile<rawrbox::ResourceTexture>("./assets/textures/rawrbox.png")->get();
		// ----

		// Textures ---
		auto mesh = rawrbox::MeshUtils::generateCube({0, 0, 0}, {3.F, 3.F, 3.F});
		mesh.setTexture(this->_texture);

		this->_model->addMesh(mesh);
		this->_model->upload();
		// ----

		this->_ready = true;
	}

	void Game::onThreadShutdown(rawrbox::ENGINE_THREADS thread) {
		if (thread == rawrbox::ENGINE_THREADS::THREAD_INPUT) {
			rawrbox::Window::shutdown();
		} else {
			this->_texture = nullptr;
			this->_model.reset();

			rawrbox::RESOURCES::shutdown();
		}
	}

	void Game::pollEvents() {
		rawrbox::Window::pollEvents();
	}

	void Game::drawWorld() {
		if (!this->_ready || this->_model == nullptr) return;
		this->_model->draw();
	}

	void Game::drawOverlay() const {
		if (!this->_ready) return;

		auto* stencil = rawrbox::RENDERER->stencil();
		stencil->drawText("Welcome to rawrbox!", {2, 2});
		stencil->render();
	}

	void Game::update() {
		rawrbox::Window::update();
		if (!this->_ready) return;

		if (this->_model != nullptr) {
			this->_model->setEulerAngle({0, rawrbox::MathUtils::toRad(rawrbox::FRAME * 0.5F), rawrbox::MathUtils::toRad(rawrbox::FRAME * 0.5F)});
		}
	}

	void Game::draw() {
		rawrbox::Window::render(); // Draw world, overlay & commit primitives
	}
} // namespace my_game

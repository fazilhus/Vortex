#pragma once
#include <Vortex.hpp>

class Sandbox2D : public Vortex::Layer {
private:

public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Vortex::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Vortex::Event& e) override;
};


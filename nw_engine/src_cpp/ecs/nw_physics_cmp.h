#ifndef ECS_PHYSICS_COMPONENT_H
#define ECS_PHYSICS_COMPONENT_H
#include <ecs/nw_entity_cmp.h>

namespace NW
{
	/// Physics2dComponent class
	class NW_API Physics2dCmp : public AEntityCmp
	{
	public:
		Physics2dCmp(AEntity& rEntity);
		~Physics2dCmp() = default;

		// --getters
		inline ACollider2dCmp* GetCollider() { return m_pCollider2d; }
		// --setters
		virtual inline void SetCollider(ACollider2dCmp* pCollider) { m_pCollider2d = pCollider; }

		// --core_methods
		virtual void OnUpdate() override;
	private:
		ACollider2dCmp* m_pCollider2d;
	};
}

#endif	// ECS_PHYSICS_COMPONENT_H
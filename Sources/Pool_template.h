#pragma once

// C�mo podr�a ser una clase de gesti�n de pools 
// La idea es que sea independiente de los datos, a esta clase no le importa el tipo de datos: solo gestiona memoria

template<class Type> class Pool_template{
public:
	bool	Init				(int count);
	Type*	GetObjectFromPool	();
	void	SendObjecToPool		(Type* obj);

	std::vector<Type *>&  GetInUseObjecs();

private:
	std::vector<Type *> FreeObjects;
	std::vector<Type *> InUseObjects;
	std::vector<Type>   Allocation;		// esta es vector<Type> para que est� toda la memoria junta

}

/* Como se usar�a:
	Pool_template<Enemy> EnemyPool;

	EnemyPool.Init(50);


	//carga de nivel
	Enemy* ene = EnemyPool.GetObjectFromPool();
	ene->init()

    std::vector<Enemy*>& listaEnemigos = EnemyPool.GetInUseObjecs();
	for(auto it = listaEnemigos.begin
	{
		it->upodate()
	}


		enemigo->End()
		listaEnemigos(enemigo);
*/
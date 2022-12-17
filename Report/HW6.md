# HW6

### Task1

第一部分的任务就是计算合力和计算速度，位移。 首先根据公式1，计算合力，结构统计在force里面，这里的主要的问题就是公式中一开始的根号造成了一点迷惑性，不过通过分析也可以了解到说应该用距离不需要根号。 

代码如下：

```cpp
void Simulator::simulate()
{
	static constexpr float dt = 0.0001f;
	static constexpr glm::vec2 gravity = glm::vec2(0.0f, -9.8f);

	// TODO: for each particle i, calculate the force f, and then update the m_v[i]
	std::vector<glm::vec2> m_v_tmp = m_v; 
	for (unsigned int i = 0; i < m_numParticles; ++i)
	{
		// Gravity force
		glm::vec2 force = gravity * m_particleMass;
		// You should use m_restLength[i][j], m_stiffness, m_x, dt, and m_particleMass herein
		for (unsigned int j = 0; j < m_numParticles; ++j)
		{
			if (i == j || m_restLength[i][j] == 0) continue;
			force += -m_stiffness * (glm::distance(m_x[i],m_x[j]) - m_restLength[i][j]) * glm::normalize(m_x[i] - m_x[j]);
		}
		//Update the m_v[i]
		m_v[i] += dt * force / m_particleMass; 

	}

	// Collide with the ground
	// Note: no need to modify it
	for (unsigned int i = 0; i < m_numParticles; ++i)
	{
		if (m_x[i].y < 0.0f)
		{
			m_x[i].y = 0.0f;
			m_v[i].y = 0.0f;
		}
		//m_v[i] *= exp(-dt * m_damping);
	}
	
	// Todo: update the position m_x[i] using m_v[i]
	// Note: you should use the time step dt
	for (unsigned int i = 1; i < m_numParticles; ++i)
	{
		// Explicit
		m_x[i] += dt * m_v_tmp[i]; 
		//Eular
		//m_x[i] += dt * m_v[i];
	}
}
```

演示视频见task1

### Task2

只用把计算的公式从v[x+1] 变成v[x]

代码如下：

```cpp
void Simulator::simulate()
{
	static constexpr float dt = 0.0001f;
	static constexpr glm::vec2 gravity = glm::vec2(0.0f, -9.8f);

	// TODO: for each particle i, calculate the force f, and then update the m_v[i]
	std::vector<glm::vec2> m_v_tmp = m_v; 
	for (unsigned int i = 0; i < m_numParticles; ++i)
	{
		// Gravity force
		glm::vec2 force = gravity * m_particleMass;
		// You should use m_restLength[i][j], m_stiffness, m_x, dt, and m_particleMass herein
		for (unsigned int j = 0; j < m_numParticles; ++j)
		{
			if (i == j || m_restLength[i][j] == 0) continue;
			force += -m_stiffness * (glm::distance(m_x[i],m_x[j]) - m_restLength[i][j]) * glm::normalize(m_x[i] - m_x[j]);
		}
		//Update the m_v[i]
		m_v[i] += dt * force / m_particleMass; 

	}

	// Collide with the ground
	// Note: no need to modify it
	for (unsigned int i = 0; i < m_numParticles; ++i)
	{
		if (m_x[i].y < 0.0f)
		{
			m_x[i].y = 0.0f;
			m_v[i].y = 0.0f;
		}
		//m_v[i] *= exp(-dt * m_damping);
	}
	
	// Todo: update the position m_x[i] using m_v[i]
	// Note: you should use the time step dt
	for (unsigned int i = 1; i < m_numParticles; ++i)
	{
		// Explicit
		//m_x[i] += dt * m_v_tmp[i]; 
		//Eular
		m_x[i] += dt * m_v[i];
	}
}
```

演示视频见task2

### Task3

这部分需要的改进也不多，主要是需要对速度做一个指数衰减

代码如下

```cpp
void Simulator::simulate()
{
	static constexpr float dt = 0.0001f;
	static constexpr glm::vec2 gravity = glm::vec2(0.0f, -9.8f);

	// TODO: for each particle i, calculate the force f, and then update the m_v[i]
	std::vector<glm::vec2> m_v_tmp = m_v; 
	for (unsigned int i = 0; i < m_numParticles; ++i)
	{
		// Gravity force
		glm::vec2 force = gravity * m_particleMass;
		// You should use m_restLength[i][j], m_stiffness, m_x, dt, and m_particleMass herein
		for (unsigned int j = 0; j < m_numParticles; ++j)
		{
			if (i == j || m_restLength[i][j] == 0) continue;
			force += -m_stiffness * (glm::distance(m_x[i],m_x[j]) - m_restLength[i][j]) * glm::normalize(m_x[i] - m_x[j]);
		}
		//Update the m_v[i]
		m_v[i] += dt * force / m_particleMass; 

	}

	// Collide with the ground
	// Note: no need to modify it
	for (unsigned int i = 0; i < m_numParticles; ++i)
	{
		if (m_x[i].y < 0.0f)
		{
			m_x[i].y = 0.0f;
			m_v[i].y = 0.0f;
		}
		m_v[i] *= exp(-dt * m_damping);
	}
	
	// Todo: update the position m_x[i] using m_v[i]
	// Note: you should use the time step dt
	for (unsigned int i = 1; i < m_numParticles; ++i)
	{
		// Explicit
		//m_x[i] += dt * m_v_tmp[i]; 
		//Eular
		m_x[i] += dt * m_v[i];
	}
}
```

演示视频见task3

### Task4

这里尝试将参数改为80和80000，原来的参数为8000，相关的演示视频见80和80000

分析：

可以看到，当k很小时会拉不动，k很大时绳子的弹力会很大，这符合物理世界的认知，k表示这根绳子的弹力性质，如果很大，则接近弹簧皮筋那样的性质，越小则越接近普通的线绳


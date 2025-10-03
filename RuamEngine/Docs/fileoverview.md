# Project structure

## /Root

- @/RuamEngine -> Entry Point


## ***/RuamCore***:


### /Files

- /FileFunctions -> Read files



### /Input

- /Input -> enums KeyCode y MousePosition, class Input



### /Maths

#### /Vec

- /Vec2, /Vec3, /Vec4 -> struct VecX in each

```cpp
    VecX(double coord1, double coord2...)

    VecX operator+, -, *, / (VecX) const
    VecX operator/ (float) const

    double Magnitude() const
    VecX Normalized() const
    VecX To(VecX target) const
    float DotProduct(VecX other) const
```



### /Objects

- /Object -> class Object:

```cpp
public:

    const ComponentVector& getComponents() const 
    template<class Comp> Comp* addComponent()  
    template<class Comp> Comp* getComponent()
```



- /Component -> class Component (abstract):  

```cpp
public: 

    Component(unsigned int obj_id) 
    virtual void Start()
    virtual void Update() 

protected:  

    unsigned int m_object_id  
``` 
  
- /Scene -> class Scene:  

```cpp
    Scene()  
```


### /Rendering

- /Camera -> class Camera:  

```cpp
    Camera()
    ~Camera()

    glm::vec3 m_position, m_direction, m_up
	float m_near_plane, m_far_plane, m_fov, m_aspect_ratio

    glm::mat4 GetProjectionMatrix() const
    glm::mat4 GetViewMatrix() const

```  

- /IndexBuffer -> class IndexBuffer:

```cpp
    IndexBuffer(const unsigned int* data, unsigned int count)
    ~IndexBuffer()

    void Bind() const
    void Unbind() const

    unsigned int GetCount() const 
```

- /Renderer -> ASSERT utils (definition GLCall()) +:
```cpp
    void GLClearError()

    bool GLLogCall(const char* function, const char* file, int line)

    class Renderer
    {
        void Clear() const;
        void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;    
    };
```

- /Shader -> class Shader:

```cpp
	    Shader(const std::string& vertexPath, const std::string& fragmentPath)
	    ~Shader()

	    void Bind() const
	    void Unbind() const

    	void SetUniform1i(const std::string& name, int value)
	    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
	    void SetUniformMat4f(const std::string& name, glm::mat4 matrix) 
```

- /Texture -> class Texture:

```cpp
    Texture(const std::string& relativePath) //(relative from @)
	~Texture()

	void Bind(unsigned int slot = 0) const
	void Unbind()

	inline int GetWidth() const
	inline int GetHeight() const
```

- /Vertex -> class Vertex:

```cpp
	Vertex()
	~Vertex()

	Vec3 position	
	Vec4 color
	Vec2 texCoords
	float texId

	static std::array<Vertex, 4> CreateQuad(float x = 0, float y = 0, float texID = 0)
```

- /VertexArray -> class VertexArray:

```cpp
	VertexArray()
	~VertexArray()

	void AddBuffer(const Buffer& vb, const VertexBufferLayout& layout)
	void Bind() const
	void Unbind() const
```

- /Buffer -> class VertexArray:

```cpp
	Buffer(const void* data, unsigned int size)
	~Buffer()

	void Bind() const
	void Unbind() const
	unsigned int GetID() const
```

- /VertexBufferLayout -> class VertexBufferLayout:

```cpp
    VertexBufferLayout()

    template<typename T> Push(unsigned int count)

    template<> Push<float>(unsigned int count) 
    template<> Push<unsigned int>(unsigned int count) 
    template<> Push<unsigned char>(unsigned int count)

    inline const std::vector<VertexBufferElement> GetElements() const&
    inline unsigned int GetStride() const
```
---

# Types


- ComponentVector = std::vector<std::shared_ptr<Component>>


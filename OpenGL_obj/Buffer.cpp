#include"pch.h"
#include<Glad/glad.h>
#include "Buffer.h"
BufferElement::BufferElement(ShaderDataType dataType, const std::string& dataName, bool Normalized)
	:count(ShaderDataTypeToCount(dataType)),size(ShaderDataTypeToSize(dataType)),type(ShaderDataTypeToGladType(dataType)),normalized(Normalized)
{

}
BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements):
	m_Elements(elements)
{
	CaculateLayout();
}
void BufferLayout::CaculateLayout()
{
	unsigned int t_offset = 0;
	m_Stride = 0;
	for (auto& element : m_Elements)
	{
		element.offset = t_offset;
		m_Stride += element.size;
		t_offset += element.size;
	}
}
VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	UnBind();
	glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::UnBind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

IndexBuffer::IndexBuffer(const void* data, unsigned int count)
	:m_Count(count)
{
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	UnBind();
	glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::UnBind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}



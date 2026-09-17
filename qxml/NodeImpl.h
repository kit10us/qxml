#pragma once

#include <io/INode.h>

namespace qxml
{
	class NodeImpl
	{
	public:
		NodeImpl(io::INode::weak_ptr node)
			: m_node {node}
		{
		}


	private:
		io::INode::weak_ptr m_node;
	};
}
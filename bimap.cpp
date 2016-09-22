#include "bimap.h"

bimap::node::node(left_t const& c, right_t const& v) :
	l_data(c),
	l_left(0),
	l_right(0),
	l_par(0),
	
	r_left(0),
	r_right(0),
	r_par(0) {
	
	r_data = v;
}

bimap::bimap() :
	l_root(0), r_root(0) {
}
bimap::~bimap() {
	while (l_root) {
		erase(begin_left());
	}
}


bimap::node* bimap::l_split(node *&root, left_t const&comp) const {
	return split<left_t>(root, comp, &node::l_data,
					&node::l_left, &node::l_right, &node::l_par);
}
bimap::node* bimap::r_split(node *&root, right_t const&comp) const {
	return split<right_t>(root, comp, &node::r_data,
					&node::r_left, &node::r_right, &node::r_par);
}

bimap::node* bimap::get_min(node *&root, node* node::*left, node* node::*right, node* node::*par) {
	if (!root) {
		return 0;
	}
	node *c = root;
	while (c->*left) {
		c = c->*left;
	}
	return c;
}
bimap::node* bimap::get_max(node *&root, node* node::*left, node* node::*right, node* node::*par) {
	if (!root) {
		return 0;
	}
	node *c = root;
	while (c->*right) {
		c = c->*right;
	}
	return c;
}

bimap::node* bimap::l_get_min(node *&root) {
	return get_min(root, &node::l_left, &node::l_right, &node::l_par);
}
bimap::node* bimap::l_get_max(node *&root) {
	return get_max(root, &node::l_left, &node::l_right, &node::l_par);
}

bimap::node* bimap::r_get_min(node *&root) {
	return get_min(root, &node::r_left, &node::r_right, &node::r_par);
}
bimap::node* bimap::r_get_max(node *&root) {
	return get_max(root, &node::r_left, &node::r_right, &node::r_par);
}

void bimap::merge(node *&first, node *second, node* node::*left, node* node::*right, 
			node* node::*par) {
	
	if (first) {
		if (second == 0) {
			return;
		}
		node *c;
		for (c = first; c->*right; c = c->*right) {
		}
		second->*par = c;
		c->*right = second;
	} else {
		first = second;
	}
}

void bimap::l_merge(node *&first, node *second) const {
	merge(first, second, &node::l_left, &node::l_right, &node::l_par);
}
void bimap::r_merge(node *&first, node *second) const {
	merge(first, second, &node::r_left, &node::r_right, &node::r_par);
}


bimap::left_iterator bimap::insert(left_t const& left, right_t const& right) {
	node *c, *v, *c1, *v1;
	
	c = l_split(l_root, left);
	v = r_split(r_root, right);
	
	c1 = l_get_max(l_root);
	v1 = r_get_max(r_root);
	
	if (((c1) && (c1->l_data == left)) || ((v1) && (v1->l_data == left))) {
		l_merge(l_root, c);
		r_merge(r_root, v);
		return end_left();
	}
	
	node *new_node;
	
	try{
		new_node = new node(left, right);
	} catch(...) {
		l_merge(l_root, c);
		r_merge(r_root, v);
		throw;
	}
	
	l_merge(l_root, new_node);
	l_merge(l_root, c);
	
	r_merge(r_root, new_node);
	r_merge(r_root, v);
	
	return left_iterator(new_node, this);
}

void bimap::erase(left_iterator it) {
	assert(l_root != 0);
	assert(it.ptr);
	
	node *c, *v;
	c = l_split(l_root, *it);
	v = l_get_max(l_root);
			
	if (v->l_par) {
		v->l_par->l_right = v->l_left;
		if (v->l_left) {
			v->l_left = v->l_par;
		}
		v->l_par = 0;
		v->l_left = 0;
	} else {
		l_root = v->l_left;
		v->l_left = 0;
		if (l_root) {
			l_root->l_par = 0;
		}
	}
	
	l_merge(l_root, c);
	///
	
	c = r_split(r_root, *it.flip());
	v = r_get_max(r_root);
	
	if (v->r_par) {
		v->r_par->r_right = v->r_left;
		if (v->r_left) {
			v->r_left = v->r_par;
		}
		v->r_par = 0;
		v->r_left = 0;
	} else {
		r_root = v->r_left;
		v->r_left = 0;
		if (r_root) {
			r_root->r_par = 0;
		}
	}
	
	r_merge(r_root, c);
	
	delete v;
}
void bimap::erase(right_iterator it) {
	erase(it.flip());
}


bimap::left_iterator  bimap::find_left (left_t  const& left)  const {
	node *c = l_split(l_root, left);
	node *v = l_get_max(l_root);
	
	l_merge(l_root, c);
	
	if (v->l_data == left) {
		return left_iterator(v, this);
	} else {
		return left_iterator(0, this);
	}
}
bimap::right_iterator bimap::find_right(right_t const& right) const {
	node *c = r_split(r_root, right);
	node *v = r_get_max(r_root);
	
	r_merge(r_root, c);
	
	if (v->r_data == right) {
		return right_iterator(v, this);
	} else {
		return right_iterator(0, this);
	}
}

bimap::left_iterator bimap::begin_left() const {
	return left_iterator(l_get_min(l_root), this);
}
bimap::left_iterator bimap::end_left() const {
	return left_iterator(0, this);
}

bimap::right_iterator bimap::begin_right() const {
	return right_iterator(r_get_min(r_root), this);
}
bimap::right_iterator bimap::end_right() const {
	return right_iterator(0, this);
}



///////////////////////////////////////////////////////////////////////////////////


bimap::left_iterator::left_iterator(node* ptr, bimap const* root) :
	ptr(ptr), root(root) {
}

bimap::left_iterator::left_iterator() :
	ptr(0), root(0) {
}

bool bimap::left_iterator::operator!=(const left_iterator& c) {
	return (ptr != c.ptr);
}
bool bimap::left_iterator::operator==(const left_iterator& c) {
	return (ptr == c.ptr);
}

bimap::left_t const& bimap::left_iterator::operator*() const {
	return ptr->l_data;
}

bimap::left_iterator& bimap::left_iterator::operator++() {
	assert(ptr);
	
	next<left_t>(ptr, root->l_root, &node::l_data, &node::l_left,
					&node::l_right, &node::l_par);
	return *this;
}
bimap::left_iterator bimap::left_iterator::operator++(int) {
	
	left_iterator res(ptr, root);
	++(*this);
	return res;
}

bimap::left_iterator& bimap::left_iterator::operator--() {
	if (!ptr) {
		ptr = l_get_max(root->l_root);
		return *this;
	}
	
	prev<left_t>(ptr, root->l_root, &node::l_data, &node::l_left,
					&node::l_right, &node::l_par);
	return *this;
	
}
bimap::left_iterator bimap::left_iterator::operator--(int) {
	left_iterator res(ptr, root);
	--(*this);
	return res;
}

bimap::right_iterator bimap::left_iterator::flip() const {
	return right_iterator(ptr, root);
}

///////////////////////////////////////////////////////////////////////////////////

bimap::right_iterator::right_iterator(node* ptr, bimap const* root) :
	ptr(ptr), root(root) {
}

bimap::right_iterator::right_iterator() :
	ptr(0), root(0) {
}

bool bimap::right_iterator::operator!=(const right_iterator& c) {
	return (ptr != c.ptr);
}
bool bimap::right_iterator::operator==(const right_iterator& c) {
	return (ptr == c.ptr);
}

bimap::right_t const& bimap::right_iterator::operator*() const {
	return ptr->r_data;
}

bimap::right_iterator& bimap::right_iterator::operator++() {
	assert(ptr);
	
	next<right_t>(ptr, root->r_root, &node::r_data, &node::r_left,
					&node::r_right, &node::r_par);
	return *this;
}
bimap::right_iterator bimap::right_iterator::operator++(int) {
	
	right_iterator res(ptr, root);
	++(*this);
	return res;
}

bimap::right_iterator& bimap::right_iterator::operator--() {
	if (!ptr) {
		ptr = r_get_max(root->r_root);
		return *this;
	}
	
	prev<right_t>(ptr, root->r_root, &node::r_data, &node::r_left,
					&node::r_right, &node::r_par);
	return *this;
}
bimap::right_iterator bimap::right_iterator::operator--(int) {
	right_iterator res(ptr, root);
	--(*this);
	return res;
}

bimap::left_iterator bimap::right_iterator::flip() const {
	return left_iterator(ptr, root);
}


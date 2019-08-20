// SPDX-License-Identifier: GPL-2.0
package algo

import "os"

type List struct {
	Head *ListNode
	Tail *ListNode
	Size int
	dtor func(interface{})
}

type ListNode struct {
	Next  *ListNode
	Value interface{}
}

func NewList(dtor func(interface{})) *List {
	if dtor == nil {
		dtor = func(interface{}) {}
	}
	return &List{dtor: dtor}
}

func (m *List) Close() {
	for n := m.Head; n != nil; n = n.Next {
		m.dtor(n.Value)
	}
}

func (m *List) InsertNext(prev *ListNode, v interface{}) (*ListNode, error) {
	node := &ListNode{Value: v}
	next := &m.Head
	if prev != nil {
		next = &prev.Next
	}
	if *next == nil || prev == m.Tail {
		m.Tail = node
	}
	node.Next = *next
	*next = node
	m.Size += 1
	return node, nil
}

func (m *List) RemoveNext(prev *ListNode) (interface{}, error) {
	node := m.Head
	if prev != nil {
		node = prev.Next
		prev.Next = nil
	}
	if node == nil {
		return 0, os.ErrInvalid
	}
	if m.Tail == node {
		m.Tail = prev
	}
	if m.Head == node {
		m.Head = node.Next
	}
	node.Next = nil
	m.Size -= 1
	return node.Value, nil
}

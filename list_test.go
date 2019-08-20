// SPDX-License-Identifier: GPL-2.0
package algo

import (
	"fmt"
	"testing"
)

func TestNewList(t *testing.T) {
	tests := []struct {
		name string
		dtor func(interface{})
		want error
	}{
		{
			name: "integer list",
			dtor: func(v interface{}) {
				value := v.(int)
				fmt.Println(value)
			},
		},
		{
			name: "string list",
			dtor: func(v interface{}) {
				value := v.(string)
				fmt.Println(value)
			},
		},
		{
			name: "binary list",
			dtor: func(v interface{}) {
				value := v.([]byte)
				fmt.Println(value)
			},
		},
	}

	for _, tc := range tests {
		tc := tc
		t.Run(tc.name, func(t *testing.T) {
			l := NewList(tc.dtor)
			if l == nil {
				t.Fatal("unexpected result")
			}
		})
	}
}

func TestListInsertNextInt(t *testing.T) {
	tests := []struct {
		name string
		dtor func(interface{})
		list []int
		want []int
		head int
		tail int
	}{
		{
			name: "acending list",
			list: []int{1, 2, 3, 4, 5},
			want: []int{1, 2, 3, 4, 5},
			head: 1,
			tail: 5,
		},
		{
			name: "decending list",
			list: []int{5, 4, 3, 2, 1},
			want: []int{5, 4, 3, 2, 1},
			head: 5,
			tail: 1,
		},
	}

	for _, tc := range tests {
		tc := tc
		t.Run(tc.name, func(t *testing.T) {
			l := NewList(tc.dtor)
			var err error
			var n *Node
			for _, v := range tc.list {
				n, err = l.InsertNext(n, v)
				if err != nil {
					t.Fatalf("unexpected error: %v", err)
				}
			}
			if got, want := l.Size, len(tc.want); got != want {
				t.Fatalf("unexpected result size:\n- want: %v\n-  got: %v",
					want, got)
			}
			if got, want := l.Head.Value.(int), tc.head; got != want {
				t.Fatalf("unexpected head value:\n- want: %v\n-  got: %v",
					want, got)
			}
			if got, want := l.Tail.Value.(int), tc.tail; got != want {
				t.Fatalf("unexpected tail value:\n- want: %v\n-  got: %v",
					want, got)
			}
			var got []int
			for n := l.Head; n != nil; n = n.Next {
				got = append(got, n.Value.(int))
			}
			for i := range got {
				if got[i] != tc.want[i] {
					t.Fatalf("unexpected %dth value:\n- want: %v\n-  got: %v",
						i, got[i], tc.want[i])
				}
			}
			l.Close()
		})
	}
}

func TestListInsertNextString(t *testing.T) {
	tests := []struct {
		name string
		dtor func(interface{})
		list []string
		want []string
		head string
		tail string
	}{
		{
			name: "ascending string",
			list: []string{"I", "II", "III", "IV", "V"},
			want: []string{"I", "II", "III", "IV", "V"},
			head: "I",
			tail: "V",
		},
		{
			name: "decending list",
			list: []string{"V", "IV", "III", "II", "I"},
			want: []string{"V", "IV", "III", "II", "I"},
			head: "V",
			tail: "I",
		},
	}

	for _, tc := range tests {
		tc := tc
		t.Run(tc.name, func(t *testing.T) {
			l := NewList(tc.dtor)
			var err error
			var n *Node
			for _, v := range tc.list {
				n, err = l.InsertNext(n, v)
				if err != nil {
					t.Fatalf("unexpected error: %v", err)
				}
			}
			if got, want := l.Size, len(tc.want); got != want {
				t.Fatalf("unexpected result size:\n- want: %v\n-  got: %v",
					want, got)
			}
			if got, want := l.Head.Value.(string), tc.head; got != want {
				t.Fatalf("unexpected head value:\n- want: %v\n-  got: %v",
					want, got)
			}
			if got, want := l.Tail.Value.(string), tc.tail; got != want {
				t.Fatalf("unexpected tail value:\n- want: %v\n-  got: %v",
					want, got)
			}
			var got []string
			for n := l.Head; n != nil; n = n.Next {
				got = append(got, n.Value.(string))
			}
			for i := range got {
				if got[i] != tc.want[i] {
					t.Fatalf("unexpected %dth value:\n- want: %v\n-  got: %v",
						i, got[i], tc.want[i])
				}
			}
			l.Close()
		})
	}
}

func TestListInsertHeadInt(t *testing.T) {
	tests := []struct {
		name string
		dtor func(interface{})
		list []int
		want []int
		head int
		tail int
	}{
		{
			name: "acending list",
			list: []int{1, 2, 3, 4, 5},
			want: []int{5, 4, 3, 2, 1},
			head: 5,
			tail: 1,
		},
		{
			name: "decending list",
			list: []int{5, 4, 3, 2, 1},
			want: []int{1, 2, 3, 4, 5},
			head: 1,
			tail: 5,
		},
	}

	for _, tc := range tests {
		tc := tc
		t.Run(tc.name, func(t *testing.T) {
			l := NewList(tc.dtor)
			var err error
			for _, v := range tc.list {
				_, err = l.InsertNext(nil, v)
				if err != nil {
					t.Fatalf("unexpected error: %v", err)
				}
			}
			if got, want := l.Size, len(tc.want); got != want {
				t.Fatalf("unexpected result size:\n- want: %v\n-  got: %v",
					want, got)
			}
			if got, want := l.Head.Value.(int), tc.head; got != want {
				t.Fatalf("unexpected head value:\n- want: %v\n-  got: %v",
					want, got)
			}
			if got, want := l.Tail.Value.(int), tc.tail; got != want {
				t.Fatalf("unexpected tail value:\n- want: %v\n-  got: %v",
					want, got)
			}
			var got []int
			for n := l.Head; n != nil; n = n.Next {
				got = append(got, n.Value.(int))
			}
			for i := range got {
				if got[i] != tc.want[i] {
					t.Fatalf("unexpected %dth value:\n- want: %v\n-  got: %v",
						i, got[i], tc.want[i])
				}
			}
			l.Close()
		})
	}
}

func TestListInsertHeadString(t *testing.T) {
	tests := []struct {
		name string
		dtor func(interface{})
		list []string
		want []string
		head string
		tail string
	}{
		{
			name: "ascending string",
			list: []string{"I", "II", "III", "IV", "V"},
			want: []string{"V", "IV", "III", "II", "I"},
			head: "V",
			tail: "I",
		},
		{
			name: "decending list",
			list: []string{"V", "IV", "III", "II", "I"},
			want: []string{"I", "II", "III", "IV", "V"},
			head: "I",
			tail: "V",
		},
	}

	for _, tc := range tests {
		tc := tc
		t.Run(tc.name, func(t *testing.T) {
			l := NewList(tc.dtor)
			var err error
			for _, v := range tc.list {
				_, err = l.InsertNext(nil, v)
				if err != nil {
					t.Fatalf("unexpected error: %v", err)
				}
			}
			if got, want := l.Size, len(tc.want); got != want {
				t.Fatalf("unexpected result size:\n- want: %v\n-  got: %v",
					want, got)
			}
			if got, want := l.Head.Value.(string), tc.head; got != want {
				t.Fatalf("unexpected head value:\n- want: %v\n-  got: %v",
					want, got)
			}
			if got, want := l.Tail.Value.(string), tc.tail; got != want {
				t.Fatalf("unexpected tail value:\n- want: %v\n-  got: %v",
					want, got)
			}
			var got []string
			for n := l.Head; n != nil; n = n.Next {
				got = append(got, n.Value.(string))
			}
			for i := range got {
				if got[i] != tc.want[i] {
					t.Fatalf("unexpected %dth value:\n- want: %v\n-  got: %v",
						i, got[i], tc.want[i])
				}
			}
			l.Close()
		})
	}
}

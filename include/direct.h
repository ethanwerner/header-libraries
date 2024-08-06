#ifndef DIRECT_H
#define DIRECT_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

struct value_t;

typedef struct edge {
  struct edge *next;
  struct node *node;
} edge_t;

typedef struct node {
  struct edge *edge;
  value_t x;
  bool flag;
  value_t (*function)(struct node *node);
} node_t;

void direct_connect(node_t *, node_t *);
void direct_disconnect(node_t *, node_t *);
void direct_update(node_t *);

#endif

#ifdef DIRECT_IMPLEMENTATION

void direct_connect(node_t *to, node_t *from) {
  edge_t *edge = malloc(sizeof(edge_t));
  edge->node = from;
  edge->next = to->edge;
  to->edge = edge;
}

void direct_disconnect(node_t *to, node_t *from) {
  edge_t *edge = to->edge;
  edge_t *last = NULL;

  while (edge) {
    if (edge->node == from) {
      edge_t *tmp = edge;
      last->next = edge->next;
      free(tmp);
    }

    last = edge;
    edge = edge->next;
  }
}

void direct_update(node_t *origin) {
  printf(">: %p - %lf\n", origin, origin->x);

  edge_t *edge = origin->edge;
  origin->flag = !origin->flag;

  while (edge) {
    if (origin->flag != edge->node->flag) {
      direct_update(edge->node);
    }

    edge = edge->next;
  }

  origin->x = origin->function(origin);
  printf("<: %p - %lf\n", origin, origin->x);
}

#endif

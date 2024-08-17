#ifndef DT_H
#define DT_H

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

void dt_connect(node_t *, node_t *);
void dt_disconnect(node_t *, node_t *);
void dt_update(node_t *);

value_t dt_one(node_t *node);
value_t dt_add(node_t *node);
value_t dt_subtract(node_t *node);

#endif

#ifdef DT_IMPLEMENTATION

void dt_connect(node_t *to, node_t *from) {
  edge_t *edge = malloc(sizeof(edge_t));
  edge->node = from;
  edge->next = to->edge;
  to->edge = edge;
}

void dt_disconnect(node_t *to, node_t *from) {
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

void dt_update(node_t *origin) {
  // printf(">: %p - %lf\n", origin, origin->x);

  edge_t *edge = origin->edge;
  origin->flag = !origin->flag;

  while (edge) {
    if (origin->flag != edge->node->flag) {
      dt_update(edge->node);
    }

    edge = edge->next;
  }

  origin->x = origin->function(origin);
  // printf("<: %p - %lf\n", origin, origin->x);
}

void dt_update_trace(node_t *origin) {
  printf(">: %p - %lf\n", origin, origin->x);

  edge_t *edge = origin->edge;
  origin->flag = !origin->flag;

  while (edge) {
    if (origin->flag != edge->node->flag) {
      dt_update_trace(edge->node);
    }

    edge = edge->next;
  }

  origin->x = origin->function(origin);
  printf("<: %p - %lf\n", origin, origin->x);
}

value_t dt_one(node_t *node) {
  return 1;
}

value_t dt_add(node_t *node) {
  edge_t *edge = node->edge;
  value_t x = node->x;

  while(edge) {
    x += edge->node->x;
    edge = edge->next;
  }

  return x;
}

value_t dt_subtract(node_t *node) {
  edge_t *edge = node->edge;
  value_t x = node->x;

  while(edge) {
    x -= edge->node->x;
    edge = edge->next;
  }

  return x;
}

#endif

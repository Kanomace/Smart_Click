/**
 * @par     Copyright (c):
 *
 *    Copyright 2019 MIoT,MI
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#ifndef __ARGUMENTS_H__
#define __ARGUMENTS_H__

#include "argument.h"

#define MAX_ARGUMENTS     5

typedef struct _arguments
{
    int           size;
    argument_t    arguments[MAX_ARGUMENTS];
} arguments_t;

arguments_t * arguments_new(void);
void arguments_delete(arguments_t *thiz);


#endif /* __ARGUMENTS_H__ */

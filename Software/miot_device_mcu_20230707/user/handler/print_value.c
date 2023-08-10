/**
 * Copyright (C) 2013-2015
 *
 * @file   print_value.c
 *
 * @remark
 *
 */

#include "print_value.h"

#include "arch_dbg.h"

void print_value(property_value_t *value)
{
    switch (value->format)
    {
        case PROPERTY_FORMAT_STRING:
            LOG_INFO("value (string): %s\n", value->data.string.value);
            break;

        case PROPERTY_FORMAT_BOOLEAN:
            LOG_INFO("value (boolean): %s\n", value->data.boolean.value ? "true" : "false");
            break;

        case PROPERTY_FORMAT_NUMBER:
            if (value->data.number.type == DATA_NUMBER_INTEGER)
            {
                LOG_INFO("value (integer): %ld\n", value->data.number.value.integerValue);
            }
            else
            {
                LOG_INFO("value (float): %f\n", value->data.number.value.floatValue);
            }
            break;

        default:
            LOG_INFO("invalid value: %d\n", value->format);
            break;
    }
}

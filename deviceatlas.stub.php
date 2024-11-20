<?php

/** @generate-class-entries */

/**
 * @var int
 * @cvalue DA_OK
 */
const DA_OK = UNKNOWN;

/**
 * @var int
 * @cvalue DA_INVALID_JSON
 */
const DA_INVALID_JSON = UNKNOWN;

/**
 * @var int
 * @cvalue DA_OVERFLOW
 */
const DA_OVERFLOW = UNKNOWN;

/**
 * @var int
 * @cvalue DA_FORMAT_ERROR
 */
const DA_FORMAT_ERROR = UNKNOWN;

/**
 * @var int
 * @cvalue DA_NOMEM
 */
const DA_NOMEM = UNKNOWN;

/**
 * @var int
 * @cvalue DA_SYS
 */
const DA_SYS = UNKNOWN;

/**
 * @var int
 * @cvalue DA_NOTIMPL
 */
const DA_NOTIMPL = UNKNOWN;

/**
 * @var int
 * @cvalue DA_NOTFOUND
 */
const DA_NOTFOUND = UNKNOWN;

/**
 * @var int
 * @cvalue DA_NOMORE
 */
const DA_NOMORE = UNKNOWN;

/**
 * @var int
 * @cvalue DA_INVALID_COOKIE
 */
const DA_INVALID_COOKIE = UNKNOWN;

/**
 * @var int
 * @cvalue DA_INVALID_TYPE
 */
const DA_INVALID_TYPE = UNKNOWN;

/**
 * @var int
 * @cvalue DA_INVALID_DATA
 */
const DA_INVALID_DATA = UNKNOWN;

/**
 * @var int
 * @cvalue DA_INTERNAL_ERROR
 */
const DA_INTERNAL_ERROR = UNKNOWN;

/**
 * @var int
 * @cvalue DA_LOCAL_JSON
 */
const DA_LOCAL_JSON = UNKNOWN;

/**
 * @var int
 * @cvalue DA_PHP_EMPTY_PATH
 */
const DA_PHP_EMPTY_PATH = UNKNOWN;

/**
 * @var int
 * @cvalue DA_PHP_BAD_PATH
 */
const DA_PHP_BAD_PATH = UNKNOWN;

/**
 * @var int
 * @cvalue DA_PHP_NO_MEM
 */
const DA_PHP_NO_MEM = UNKNOWN;

function deviceatlas_get_properties(array $headers): array|false {}
function deviceatlas_last_error(): int {}
function deviceatlas_last_error_msg(): string {}

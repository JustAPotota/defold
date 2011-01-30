#! /usr/bin/env python

VERSION='0.1'
APPNAME='gamesys'

srcdir = '.'
blddir = 'build'

import sys, os
import waf_ddf, waf_graphics, waf_dynamo, waf_physics, waf_render

def init():
    pass

def set_options(opt):
    opt.sub_options('src')
    opt.tool_options('compiler_cc')
    opt.tool_options('compiler_cxx')
    opt.tool_options('waf_dynamo')

def configure(conf):
    conf.check_tool('compiler_cc')
    conf.check_tool('compiler_cxx')
    conf.check_tool('java')
    conf.check_tool('waf_dynamo')

    waf_ddf.configure(conf)
    waf_graphics.configure(conf)
    waf_physics.configure(conf)
    waf_render.configure(conf)

    if sys.platform == "darwin":
        platform = "darwin"
    elif sys.platform == "linux2":
        platform = "linux"
    elif sys.platform == "win32":
        platform = "win32"
    else:
        conf.fatal("Unable to determine platform")

    if platform == "darwin":
        conf.env.append_value('LINKFLAGS', ['-framework', 'Cocoa', '-framework', 'OpenGL', '-framework', 'OpenAL', '-framework', 'AGL', '-framework', 'IOKit', '-framework', 'Carbon'])
    if platform == "linux":
        conf.env.append_value('LINKFLAGS', ['-lglut', '-lXext', '-lX11', '-lXi', '-lGL', '-lGLU', '-lpthread'])
    if platform == "win32":
        conf.env.append_value('LINKFLAGS', ['opengl32.lib', 'user32.lib'])

    conf.env.append_value('CPPPATH', "default/src")
    conf.env['LIB_GTEST'] = 'gtest'
    conf.env['STATICLIB_DLIB'] = 'dlib'
    conf.env['STATICLIB_DDF'] = 'ddf'
    conf.env['STATICLIB_LUA'] = 'lua'
    conf.env['STATICLIB_RESOURCE'] = 'resource'
    conf.env['STATICLIB_GAMEOBJECT'] = 'gameobject'
    conf.env['STATICLIB_LUA'] = 'lua'
    conf.env['STATICLIB_SCRIPT'] = 'script'
    conf.env['STATICLIB_RENDER'] = 'render'
    conf.env['STATICLIB_GRAPHICS_NULL'] = 'graphics_null'
    conf.env['STATICLIB_PARTICLE'] = 'particle'
    conf.env['STATICLIB_GUI'] = 'gui'
    conf.env['STATICLIB_SOUND_NULL'] = 'sound_null'
    conf.env['STATICLIB_INPUT'] = 'input'
    conf.env['STATICLIB_HID'] = 'hid'
    conf.env['STATICLIB_HID_NULL'] = 'hid_null'
    conf.env['STATICLIB_GRAPHICS'] = 'graphics'
    conf.env['STATICLIB_DMGLFW'] = 'dmglfw'

    platform = conf.env['PLATFORM']

    if platform == "linux":
        conf.env['LIB_PLATFORM_SOCKET'] = ''
    elif 'darwin' in platform:
        conf.env['LIB_PLATFORM_SOCKET'] = ''
    else:
        conf.env['LIB_PLATFORM_SOCKET'] = 'WS2_32'

    conf.env.append_unique('CCDEFINES', 'DLIB_LOG_DOMAIN="GAMESYS"')
    conf.env.append_unique('CXXDEFINES', 'DLIB_LOG_DOMAIN="GAMESYS"')

    conf.sub_config('src')

def build(bld):
    sys.path.insert(0, bld.path.find_dir('src').abspath())
    sys.path.insert(0, os.path.abspath('build/default/proto'))

    bld.add_subdirs('content')
    bld.add_group()
    bld.add_subdirs('src')

    bld.install_files('${PREFIX}/bin', 'tools/meshc.py', chmod=493)

import Build, Options
import os, subprocess
def shutdown():
    if not Options.commands['build']:
        return
    waf_dynamo.run_gtests(valgrind = True)

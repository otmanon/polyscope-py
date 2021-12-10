import polyscope_bindings as psb
import os
import numpy as np

from polyscope.core import str_to_datatype, glm3

def remove_floating_quantity(name, error_if_absent=False):
    psb.remove_floating_quantity(name, error_if_absent)

def remove_all_floating_quantities():
    psb.remove_all_floating_quantities()

def add_floating_scalar_image(name, dims, values, enabled=None, datatype="standard", vminmax=None, cmap=None, show_fullscreen=None):

    # Check dimensions
    if len(dims) != 2:
        raise ValueError("`dims` should be a tuple of (width,height) pixel dimensions")
    w = dims[0]
    h = dims[1]

    # Check image data
    shape_msg = "`values` should be a 2d [width,height] or 1d [width*height] numpy array"
    if len(values.shape) == 1:
        # (if it is a width x height array, flatten it)
        if(values.shape[0] != w*h): raise ValueError(shape_msg)
    elif len(values.shape) == 2:
        if(values.shape[0] != w or values.shape[1] != h): raise ValueError(shape_msg)
        values = values.flatten() # flatten it
    else:
        raise ValueError(shape_msg)

    # Add the actual quantity
    q = psb.add_floating_scalar_image(name, w, h, values, str_to_datatype(datatype))

    # Support optional params
    if enabled is not None:
        q.set_enabled(enabled)
    if vminmax is not None:
        q.set_map_range(vminmax)
    if cmap is not None:
        q.set_color_map(cmap)
    if show_fullscreen is not None:
        q.set_show_fullscreen(show_fullscreen)

def remove_floating_scalar_image(name):
    psb.remove_floating_scalar_image(name)

def add_floating_color_image(name, dims, values, enabled=None, show_fullscreen=None):

    # Check dimensions
    if len(dims) != 2:
        raise ValueError("`dims` should be a tuple of (width,height) pixel dimensions")
    w = dims[0]
    h = dims[1]

    # Check image data
    shape_msg = "`values` should be a 3d [width,height,3] or 2d [width*height,3] numpy array"
    if len(values.shape) == 2:
        # (if it is a width x height array, flatten it)
        if(values.shape[0] != w*h or values.shape[1] != 3): raise ValueError(shape_msg)
    elif len(values.shape) == 3:
        if(values.shape[0] != w or values.shape[1] != h or values.shape[2] != 3): raise ValueError(shape_msg)
        values = values.reshape((w*h, 3)) # flatten it
    else:
        raise ValueError(shape_msg)

    # Add the actual quantity
    q = psb.add_floating_color_image(name, w, h, values)

    # Support optional params
    if enabled is not None:
        q.set_enabled(enabled)
    if show_fullscreen is not None:
        q.set_show_fullscreen(show_fullscreen)

def remove_floating_color_image(name):
    psb.remove_floating_color_image(name)

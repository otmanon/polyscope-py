import polyscope_bindings as psb

from polyscope.core import str_to_datatype, str_to_vectortype, glm3

class VolumeGrid:

    # This class wraps a _reference_ to the underlying object, whose lifetime is managed by Polyscope

    # End users should not call this constrctor, use register_volume_grid instead
    def __init__(self, name=None, size=None, bound_min=None, bound_max=None, instance=None):

        if instance is not None:
            # Wrap an existing instance
            self.bound_grid = instance

        else:
            # Create a new instance
            if isinstance(size, int) or (isinstance(size, tuple) and len(size) == 3):
                self.bound_grid = psb.register_volume_grid(name, size, glm3(bound_min), glm3(bound_max))
            else:
                raise ValueError("Volume grid `size` should be an integer or length-3 tuple of integers")

    def n_values(self):
        return self.bound_grid.n_values()
    
    def position_of_index(self, ind):
        return self.bound_grid.position_of_index(ind)

    ## Structure management
    
    def remove(self):
        '''Remove the structure itself'''
        self.bound_grid.remove()
    def remove_all_quantities(self):
        '''Remove all quantities on the structure'''
        self.bound_grid.remove_all_quantities()
    def remove_quantity(self, name, error_if_absent=False):
        '''Remove a single quantity on the structure'''
        self.bound_grid.remove_quantity(name, error_if_absent)

    # Enable/disable
    def set_enabled(self, val=True):
        self.bound_grid.set_enabled(val)
    def is_enabled(self):
        return self.bound_grid.is_enabled()
    
    # Transparency
    def set_transparency(self, val):
        self.bound_grid.set_transparency(val)
    def get_transparency(self):
        return self.bound_grid.get_transparency()
   
    # Transformation things
    def center_bounding_box(self):
        self.bound_grid.center_bounding_box()
    def rescale_to_unit(self):
        self.bound_grid.rescale_to_unit()
    def reset_transform(self):
        self.bound_grid.reset_transform()
    def set_transform(self, new_mat4x4):
        self.bound_grid.set_transform(new_mat4x4)
    def set_position(self, new_vec3):
        self.bound_grid.set_position(new_vec3)
    def translate(self, trans_vec3):
        self.bound_grid.translate(trans_vec3)
    def get_transform(self):
        return self.bound_grid.get_transform()
    def get_position(self):
        return self.bound_grid.get_position()

    # Slice planes
    # def set_cull_whole_elements(self, val):
        # self.bound_grid.set_cull_whole_elements(val)
    # def get_cull_whole_elements(self):
        # return self.bound_grid.get_cull_whole_elements()
    def set_ignore_slice_plane(self, plane, val):
        # take either a string or a slice plane object as input
        if isinstance(plane, str):
            self.bound_grid.set_ignore_slice_plane(plane, val)
        else:
            self.bound_grid.set_ignore_slice_plane(plane.get_name(), val)
    def get_ignore_slice_plane(self, plane):
        # take either a string or a slice plane object as input
        if isinstance(plane, str):
            return self.bound_grid.get_ignore_slice_plane(plane)
        else:
            return self.bound_grid.get_ignore_slice_plane(plane.get_name())

    ## Options
   
    # Material
    def set_material(self, mat):
        self.bound_grid.set_material(mat)
    def get_material(self):
        return self.bound_grid.get_material()


    ## Quantities
       
    # Scalar
    def add_scalar_quantity(self, name, values, enabled=None, datatype="standard", vminmax=None, cmap=None, point_viz_enabled=None, isosurface_viz_enabled=None, isosurface_level=None, isosurface_color=None):
        if len(values.shape) != 1 or values.shape[0] != self.n_values(): raise ValueError("'values' should be a length-N array")
            
        q = self.bound_grid.add_scalar_quantity(name, values, str_to_datatype(datatype))

        # Support optional params
        if enabled is not None:
            q.set_enabled(enabled)
        if vminmax is not None:
            q.set_map_range(vminmax)
        if cmap is not None:
            q.set_color_map(cmap)
        if point_viz_enabled is not None:
            q.set_point_viz_enabled(point_viz_enabled)
        if isosurface_viz_enabled is not None:
            q.set_isosurface_viz_enabled(isosurface_viz_enabled)
        if isosurface_level is not None:
            q.set_isosurface_level(isosurface_level)
        if isosurface_color is not None:
            q.set_isosurface_color(glm3(isosurface_color))
   
    # Scalar from function
    def add_scalar_quantity_from_callable(self, name, func, enabled=None, datatype="standard", vminmax=None, cmap=None, point_viz_enabled=None, isosurface_viz_enabled=None, isosurface_level=None, isosurface_color=None):

        q = self.bound_grid.add_scalar_quantity_from_callable(name, func, str_to_datatype(datatype))

        # Support optional params
        if enabled is not None:
            q.set_enabled(enabled)
        if vminmax is not None:
            q.set_map_range(vminmax)
        if cmap is not None:
            q.set_color_map(cmap)
        if point_viz_enabled is not None:
            q.set_point_viz_enabled(point_viz_enabled)
        if isosurface_viz_enabled is not None:
            q.set_isosurface_viz_enabled(isosurface_viz_enabled)
        if isosurface_level is not None:
            q.set_isosurface_level(isosurface_level)
        if isosurface_color is not None:
            q.set_isosurface_color(glm3(isosurface_color))
    
    # Scalar from function
    def add_scalar_quantity_from_batch_callable(self, name, func, enabled=None, datatype="standard", vminmax=None, cmap=None, point_viz_enabled=None, isosurface_viz_enabled=None, isosurface_level=None, isosurface_color=None):

        q = self.bound_grid.add_scalar_quantity_from_batch_callable(name, func, str_to_datatype(datatype))

        # Support optional params
        if enabled is not None:
            q.set_enabled(enabled)
        if vminmax is not None:
            q.set_map_range(vminmax)
        if cmap is not None:
            q.set_color_map(cmap)
        if point_viz_enabled is not None:
            q.set_point_viz_enabled(point_viz_enabled)
        if isosurface_viz_enabled is not None:
            q.set_isosurface_viz_enabled(isosurface_viz_enabled)
        if isosurface_level is not None:
            q.set_isosurface_level(isosurface_level)
        if isosurface_color is not None:
            q.set_isosurface_color(glm3(isosurface_color))



def register_volume_grid(name, size, bound_min, bound_max, enabled=None, material=None, transparency=None):
    """Register a new volume grid"""

    p = VolumeGrid(name, size, bound_min, bound_max)

    # == Apply options
    if enabled is not None:
        p.set_enabled(enabled)
    if material is not None:
        p.set_material(material)
    if transparency is not None:
        p.set_transparency(transparency)

    return p

def remove_volume_grid(name, error_if_absent=True):
    """Remove a volume grid by name"""
    psb.remove_volume_grid(name, error_if_absent)

def get_volume_grid(name):
    """Get volume grid by name"""
    if not has_volume_grid(name):
        raise ValueError("no volume grid with name " + str(name))

    raw_cloud = psb.get_volume_grid(name)

    # Wrap the instance
    return VolumeGrid(instance=raw_cloud)

def has_volume_grid(name):
    """Check if a volume grid exists by name"""
    return psb.has_volume_grid(name)
    

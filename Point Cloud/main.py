import open3d as o3d
import numpy as np

# --------------------------- 加载点云 ---------------------------
print("->正在加载点云... ")
pcd = o3d.io.read_point_cloud("./resource/bunny.ply")
print("原始点云：", pcd)
# ==============================================================

# ------------------------- Alpha shapes -----------------------
alpha = 0.03
print(f"alpha={alpha:.3f}")
mesh = o3d.geometry.TriangleMesh.create_from_point_cloud_alpha_shape(pcd, alpha)
mesh.compute_vertex_normals()
o3d.visualization.draw_geometries([mesh], mesh_show_back_face=True)
# ==============================================================
#
#
#
#
#
#
#
# import open3d as o3d
# import numpy as np
#
# # ---------------------- 定义点云体素化函数 ----------------------
# def get_mesh(_relative_path):
#     mesh = o3d.io.read_triangle_mesh(_relative_path)
#     mesh.compute_vertex_normals()
#     return mesh
# # =============================================================
#
# # ------------------------- Ball pivoting --------------------------
# print("->Ball pivoting...")
# _relative_path = "./resource/bunny.ply"    # 设置相对路径
# N = 6000                        # 将点划分为N个体素
# pcd = get_mesh(_relative_path).sample_points_poisson_disk(N)
# o3d.visualization.draw_geometries([pcd])
#
# radii = [0.005, 0.01, 0.02, 0.04]
# rec_mesh = o3d.geometry.TriangleMesh.create_from_point_cloud_ball_pivoting(pcd, o3d.utility.DoubleVector(radii))
# o3d.visualization.draw_geometries([pcd, rec_mesh])
# # ==============================================================

#
#
#
# import open3d as o3d
# import numpy as np
#
# # --------------------------- 加载点云 ---------------------------
# print("->正在加载点云... ")
# pcd = o3d.io.read_point_cloud("./resource/bunny.ply")
# print("原始点云：", pcd)
# # ==============================================================
#
# # ------------------------- Ball pivoting --------------------------
# print('run Poisson surface reconstruction')
# radius = 0.001   # 搜索半径
# max_nn = 30         # 邻域内用于估算法线的最大点数
# pcd.estimate_normals(search_param=o3d.geometry.KDTreeSearchParamHybrid(radius, max_nn))     # 执行法线估计
#
# with o3d.utility.VerbosityContextManager(o3d.utility.VerbosityLevel.Debug) as cm:
#     mesh, densities = o3d.geometry.TriangleMesh.create_from_point_cloud_poisson(pcd, depth=12)
# print(mesh)
# o3d.visualization.draw_geometries([mesh])
# # ==============================================================



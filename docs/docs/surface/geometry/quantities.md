This page enumerates the surface geometry quantities available in geometry central.

Recall that these quantities are each associated with a [geometry interface](geometry.md#geometry-hierarchy) specifying what can be computed from the given input data. Instantiating a geometry from data, like a `VertexPositionGeometry` extends these interfaces and gives access to all of the quantities therein.  Quantities should usually be accessed via the [managed caches](geometry.md#managed-quantities), as in the example below.

```cpp
#include "geometrycentral/surface/geometry.h"
#include "geometrycentral/surface/meshio.h"
using namespace geometrycentral::surface;

// Load a mesh and geometry from file
std::unique_ptr<HalfedgeMesh> mesh;
std::unique_ptr<VertexPositionGeometry> positionGeometry;
std::tie<mesh, positionGeometry> = loadMesh("spot.obj");

// For the sake of the example, bind to an interface that offers
// only the quantities which we will actually use below.
IntrinsicGeometry& geometry = *positionGeometry;

// populate the quantity
geometry.requireFaceAreas();

for(Face f : mesh->faces()) {

  // access managed array holding quantity
  double area = geometry.faceAreas[f];

  // immediate computation: generally discouraged
  area = geometry.computeFaceArea(f);
}
```

## Lengths, areas, and angles

These quantities are defined for any `IntrinsicGeometry`, which is the base class of all other geometry objects---they will always be available on any kind of geometry.

??? func "edge length"
    
    ##### edge length

    The length of an edge in the mesh, as a non-negative real number.

    - **member:** `EdgeData<double> IntrinsicGeometry::edgeLengths`
    - **require:** `void IntrinsicGeometry::requireEdgeLengths()`
    - **immediate:** `double IntrinsicGeometry::computeEdgeLength(Edge e)`

??? func "face area"
    
    ##### face area

    The area of a face, as a non-negative real number.

    May be computed from edge lengths via Heron's formula, or from embedded vertex positions with a cross product.

    Only valid on triangular meshes.

    - **member:** `FaceData<double> IntrinsicGeometry::faceAreas`
    - **require:** `void IntrinsicGeometry::requireFaceAreas()`
    - **immediate:** `double IntrinsicGeometry::computeFaceAreas(Face f)`

??? func "vertex dual area"

    ##### vertex dual area

    An area associated with each vertex, as a non-negative real number.

    Only valid on triangular meshes.

    Defined to be $1/3$ the sum of all adjacent face areas. The sum of all vertex dual areas is equal to the usual surface area of the mesh.

    - **member:** `VertexData<double> IntrinsicGeometry::vertexDualAreas`
    - **require:** `void IntrinsicGeometry::requireVertexDualAreas()`

??? func "corner angles"
    
    ##### corner angles

    The angle between incident edges at each corner of a mesh.

    Only valid on triangular meshes.

    - **member:** `CornerData<double> IntrinsicGeometry::cornerAngles`
    - **require:** `void IntrinsicGeometry::requireCornerAngles()`
    - **immediate:** `double IntrinsicGeometry::computeCornerAngle(Corner c)`

??? func "corner scaled angles"
    
    ##### corner scaled angles

    The angle between incident edges at each corner of a mesh, linearly rescaled such that the angles around every vertex sum to $2 \pi$. At boundary vertices, no scaling will be performed.

    Only valid on triangular meshes.

    - **member:** `CornerData<double> IntrinsicGeometry::cornerScaledAngles`
    - **require:** `void IntrinsicGeometry::requireCornerScaledAngles()`

??? func "vertex angle sum"
    
    ##### vertex angle sum

    The sum of corner angles around a vertex.

    Only valid on triangular meshes.

    - **member:** `VertexData<double> IntrinsicGeometry::vertexAngleSums`
    - **require:** `void IntrinsicGeometry::requireVertexAngleSums()`

??? func "vertex Gaussian curvature"
    
    ##### vertex Gaussian curvature

    The [_Gaussian curvature_](https://en.wikipedia.org/wiki/Gaussian_curvature) $K$ at a vertex, defined via the angle defect $K_v = 2 \pi - \sum \theta_i$, where $\sum \theta_i$ is the `vertexAngleSum` as above.

    Should be interpreted as an _integrated_ Gaussian curvature, giving the total curvature in the neighborhood of the vertex. On a closed surface, the [Gauss-Bonnet theorem](https://en.wikipedia.org/wiki/Gauss%E2%80%93Bonnet_theorem) tells us that the sum of these Gaussian curvatures will be a topological constant given by $\sum_v K_v = 2 \pi \chi$, where $\chi$ is the [Euler characteristic](../halfedge_mesh/basics.md#properties) of the surface. On surfaces with boundary, the geodesic curvature of the boundary factors in.

    Only valid on triangular meshes.

    - **member:** `VertexData<double> IntrinsicGeometry::vertexGaussianCurvatures`
    - **require:** `void IntrinsicGeometry::requireVertexGaussianCurvatures()`

??? func "face Gaussian curvature"
    
    ##### face Gaussian curvature

    The [_Gaussian curvature_](https://en.wikipedia.org/wiki/Gaussian_curvature) $K$ at a face, defined via the rescaled angle defect in the face $K_f = \pi - \sum \tilde{\theta}_i$, where $\tilde{\theta}_i$ are the _rescaled_ corner angles (as in `cornerScaledAngles`) incident on the face.

    Should be interpreted as an _integrated_ Gaussian curvature, giving the total curvature inside of the face. A corresponding curvature-per-unit-area can be computed by dividing by the area of the face.

    On a closed surface, the [Gauss-Bonnet theorem](https://en.wikipedia.org/wiki/Gauss%E2%80%93Bonnet_theorem) tells us that the sum of these Gaussian curvatures will be a topological constant given by $\sum_f K_f = 2 \pi \chi$, where $\chi$ is the [Euler characteristic](../halfedge_mesh/basics.md#properties) of the surface. On surfaces with boundary, the geodesic curvature of the boundary factors in.

    Only valid on triangular meshes.

    - **member:** `FaceData<double> IntrinsicGeometry::faceGaussianCurvatures`
    - **require:** `void IntrinsicGeometry::requireFaceGaussianCurvatures()`

??? func "halfedge cotan weight"
    
    ##### halfedge cotan weight

    The "cotangent weight" of an interior halfedge, defined as $\frac{1}{2} \cot(\theta)$, where $\theta$ is the corner angle opposite the halfedge. Defined to be $0$ for exterior halfedges.

    Can be computed directly from edge lengths, or more efficiently in an embedded triangle via $\cot(\theta) = \frac{u \cdot v}{||u \times v||}$, where $u$ and $v$ are the edge vectors emanating from the opposite corner.

    Only valid on triangular meshes.

    - **member:** `HalfedgeData<double> IntrinsicGeometry::halfedgeCotanWeights`
    - **require:** `void IntrinsicGeometry::requireHalfedgeCotanWeights()`


??? func "edge cotan weight"

    ##### edge cotan weight

    The "cotangent weight" of an edge, defined as the sum of halfedge cotan weights for incident interior halfedges.

    Only valid on triangular meshes.

    - **member:** `EdgeData<double> IntrinsicGeometry::edgeCotanWeights`
    - **require:** `void IntrinsicGeometry::requireEdgeCotanWeights()`


## Tangent vectors and transport

These quantities are defined for any `IntrinsicGeometry`, which is the base class of all other geometry objects---they will always be available on any kind of geometry. Tangent vectors and transport are defined in terms of tangent spaces at faces and vertices, as defined below.

Recall that our `Vector2` types obey the multiplication and division rules of complex arithmetic, and thus can be used to represent rotations. For instance, a 2D vector representing a rotation can be used to rotate another vector like:
```cpp
Vector2 v = /* your vector */
Vector2 r = Vector2{std::cos(PI/4), std::sin(PI/4)}; // rotation by 45 degrees
Vector2 vRot = r * v;
```
This is fundamentally no different from using 2x2 rotation matrices, but leads to much cleaner code (try using division to compute relative rotations!).

#### Face tangent spaces

To represent vectors that sit in flat mesh faces, we define a 2D coordinate frame tangent to each face. By default, this frame is aligned such that `face.halfedge()` points along the $x$-axis. All vectors in faces are then expressed via $(x,y)$ `Vector2D` coordinates in this frame. Crucially, this basis is well-defined even if the geometry does not have vertex positions.

See [face tangent basis](#face-tangent-basis) to convert these vectors to world coordinates (if your mesh has vertex positions).

??? func "halfedge vectors in face"
    
    ##### halfedge vectors in face

    Vectors for each halfedge in the coordinate frame of the face in which they sit. See the description of face tangent spaces above for a definition.

    Only valid on triangular meshes.

    - **member:** `HalfedgeData<Vector2> IntrinsicGeometry::halfedgeVectorsInFace`
    - **require:** `void IntrinsicGeometry::requireHalfedgeVectorsInFace()`


??? func "transport vector across halfedge"
    
    ##### transport vector across halfedge

    Rotations which transport tangent vectors **across** a halfedge, rotating a vector from the tangent space of `halfedge.face()` to the tangent space `halfedge.twin().face()`.

    Always a unit vector, which can be multiplied by any other vector to compute the rotation. (recall our `Vector2`s multiply like complex numbers)

    Only valid on triangular meshes. Not defined for halfedges (interior or exterior) incident on boundary edges, these boundary values are set to NaN so errors can be caught quickly.

    - **member:** `HalfedgeData<Vector2> IntrinsicGeometry::transportVectorAcrossHalfedge`
    - **require:** `void IntrinsicGeometry::requireTransportVectorAcrossHalfedge()`
    
    Example usage:
    ```cpp
    geometry.requireTransportVectorAcrossHalfedge();

    Face f = /* ... */;        // a face of interest
    Vector2 myVec = /* ... */; // tangent vector in face f
    
    for(Halfedge he : f.adjacentHalfedges()) {

      Vertex neighborFace = he.twin().face();
      Vector2 rot = geometry.transportVectorAcrossHalfedge[he];
      Vector2 neighVec = rot * myVec;    // now in the basis of neighborFace
    }

    ```


#### Vertex tangent spaces

To represent vectors that sit at mesh faces, we consider a polar coordinate frame at each vertex. This frame is defined by measuring angles according to the rescaled corner angles as in `cornerScaledAngles`. By default, this frame is aligned such that `vertex.halfedge()` points along the $\phi=0$ $x$-axis. Of course, rather than using polar coordinates we can equivalently work in the corresponding Cartesian frame---tangent vectors at vertices are then expressed via $(x,y)$ `Vector2D` coordinates in this frame. Crucially, this basis does not require picking a vertex normal, and is well-defined even if the geometry does not have vertex positions.

See [vertex tangent basis](#vertex-tangent-basis) to convert these tangent vectors to world coordinates (if your mesh has vertex positions).

![vertex tangent coordinates diagram](../../../media/vertex_tangent_coordinates.svg)


??? func "halfedge vectors in vertex"
    
    ##### halfedge vectors in vertex

    Vectors for each halfedge in the coordinate frame of the vertex from which the emanate (in `halfedge.vertex()`). See the description of vertex tangent spaces above for a definition.

    Only valid on triangular meshes.

    - **member:** `HalfedgeData<Vector2> IntrinsicGeometry::halfedgeVectorsInVertex`
    - **require:** `void IntrinsicGeometry::requireHalfedgeVectorsInVertex()`


??? func "transport vector along halfedge"
    
    ##### transport vector along halfedge

    Rotations which transport tangent vectors **along** a halfedge, rotating a vector from the tangent space of `halfedge.vertex()` to the tangent space `halfedge.twin().vertex()`.

    Always a unit vector, which can be multiplied by any other vector to compute the rotation. (recall our `Vector2`s multiply like complex numbers)

    Only valid on triangular meshes.

    - **member:** `HalfedgeData<Vector2> IntrinsicGeometry::transportVectorAlongHalfedge`
    - **require:** `void IntrinsicGeometry::requireTransportVectorAlongHalfedge()`
    
    Example usage:
    ```cpp
    geometry.requireTransportVectorAlongHalfedge();

    Vertex v = /* ... */;        // a vertex of interest
    Vector2 myVec = /* ... */;   // tangent vector in vertex v
    
    for(Halfedge he : v.outgoingHalfedges()) {
      Vertex neighborVertex = he.twin().vertex();
      Vector2 rot = geometry.transportVectorAlongHalfedge[he];
      Vector2 neighVec = rot * myVec;    // now in the basis of neighborVertex
    }

    ```


## Operators


These quantities are defined for any `IntrinsicGeometry`, which is the base class of all other geometry objects---they will always be available on any kind of geometry. A full explanation of these operators is beyond the scope of these docs; see [these course notes](https://www.cs.cmu.edu/~kmcrane/Projects/DDG/paper.pdf) for one introduction.

All operators are indexed over mesh elements according to the natural iteration order of the elements, or equivalently the indices from `HalfedgeMesh::getVertexIndices()` (etc).

??? func "cotangent Laplacian"
    
    ##### cotangent laplacian

    The discrete Laplace operator, discretized via cotangent weights.

    A $|V| \times |V|$ real matrix. Always symmetric and positive semi-definite. If and only the underlying geometry is _Delaunay_, the matrix will furthermore have all negative off-diagonal entries, satisfy a maximum principle, and be an _M-matrix_.

    This is the _weak_ Laplace operator, if we use it to evalutae $\mathsf{y} \leftarrow \mathsf{L} \mathsf{x}$, $\mathsf{x}$ should hold _pointwise_ quantities at vertices, and the result $\mathsf{y}$ will contain _integrated_ values of the result in the neighborhood of each vertex. If used to solve a Poisson problem, a mass matrix (such as the lumped or Galerkin mass matrices below) are likely necessary on the right hand side.

    Only valid on triangular meshes.

    - **member:** `Eigen::SparseMatrix<double> IntrinsicGeometry::laplacian`
    - **require:** `void IntrinsicGeometry::requireLaplacian()`

??? func "vertex lumped mass matrix"

    ##### vertex lumped mass matrix

    A mass matrix at vertices, where vertex area is $1/3$ the incident face areas as in `vertexDualAreas`.

    A $|V| \times |V|$ real diagonal matrix. Generally less-accurate than the Galerkin mass matrix below, but can be easily inverted since it is a diagonal matrix.

    Only valid on triangular meshes.

    - **member:** `Eigen::SparseMatrix<double> IntrinsicGeometry::vertexLumpedMassMatrix`
    - **require:** `void IntrinsicGeometry::requireVertexLumpedMassMatrix()`


??? func "vertex Galerkin mass matrix"

    ##### vertex Galerkin mass matrix

    A mass matrix at vertices, supported at all neighbors of a vertex via integration of piecewise-linear elements.

    A $|V| \times |V|$ real matrix. Generally more accurate than the lumped mass matrix above, should be preferred unless the mass matrix needs to be inverted.

    Only valid on triangular meshes.

    - **member:** `Eigen::SparseMatrix<double> IntrinsicGeometry::vertexGalerkinMassMatrix`
    - **require:** `void IntrinsicGeometry::requireVertexGalerkinMassMatrix()`

??? func "vertex connection Laplacian"

    ##### vertex connection Laplacian

    A discrete connection Laplacian operator, which applies to vector fields defined in vertex tangent spaces. Essentially defined as the scalar cotangent Laplacian, augmented with rotations given by the rotations in `transportVectorAlongHalfedge`; see [The Vector Heat Method, Sec 5.3](http://www.cs.cmu.edu/~kmcrane/Projects/VectorHeatMethod/paper.pdf) for more explanation and definition.

    A $|V| \times |V|$ complex matrix. Always Hermitian, but positive semi-definite if and only the underlying geometry is _Delaunay_.  This is a _weak_ Laplace operator, the application of which outputs integrated values in vertex neighborhood.

    Given a complex vector $\mathsf{x}$ of tangent vectors at vertices, apply the operator by multiplying $\mathsf{L} * \mathsf{x}$.

    Only valid on triangular meshes.

    - **member:** `Eigen::SparseMatrix<double> IntrinsicGeometry::vertexGalerkinMassMatrix`
    - **require:** `void IntrinsicGeometry::requireVertexGalerkinMassMatrix()`

??? func "DEC operators"

    ##### DEC operators

    These operators are the basic building blocks for _discrete exterior calculus_ on surfaces.

    **Note:** These quantities slightly deviate from the usual naming scheme for quantities. Rather than `requireD0()`, `requireD1()`, etc, there is a single `requireDECOperators()` function which manages all 8 of the members listed below.

    The following members are constructed:

    - `Eigen::SparseMatrix<double> IntrinsicGeometry::hodge0` A $|V| \times |V|$ diagonal matrix
    - `Eigen::SparseMatrix<double> IntrinsicGeometry::hodge0Inverse` A $|V| \times |V|$ diagonal matrix
    - `Eigen::SparseMatrix<double> IntrinsicGeometry::hodge1` An $|E| \times |E|$ diagonal matrix
    - `Eigen::SparseMatrix<double> IntrinsicGeometry::hodge1Inverse` An $|E| \times |E|$ diagonal matrix
    - `Eigen::SparseMatrix<double> IntrinsicGeometry::hodge2` An $|F| \times |F|$ diagonal matrix
    - `Eigen::SparseMatrix<double> IntrinsicGeometry::hodge2Inverse` An $|F| \times |F|$ diagonal matrix
    - `Eigen::SparseMatrix<double> IntrinsicGeometry::d0` An $|E| \times |V|$ matrix with $\{-1, 0, 1\}$ entries
    - `Eigen::SparseMatrix<double> IntrinsicGeometry::d1` An $|F| \times |E|$ matrix with $\{-1, 0, 1\}$ entries

    Only valid on triangular meshes.

    - **require:** `void IntrinsicGeometry::requireDECOperators()`


## Extrinsic angles

These quantities depend on extrinsic angles, but are still rotation-invariant, and independent of a particular embeddeding. They are defined for `ExtrinsicGeometry` and classes that extend it, including the `EmbeddedGeometry` one usually constructs from vertex positions. Currently there is no realization that constructs an `ExtrinsicGeometry` from input data which is not also an `EmbeddedGeometry`, but such a class could be implemented in the future.


??? func "edge dihedral angle"

    ##### edge dihedral angle 

    The dihedral angle at an edge, in radians. Defined to be $\pi$ if the edge is flat, $ < \pi$ at a convex edge, and $ > \pi$ at a nonconvex edge.

    Only valid on triangular meshes.

    - **member:** `EdgeData<double> ExtrinsicGeometry::edgeDihedralAngles`
    - **require:** `void ExtrinsicGeometry::requireEdgeDihedralAngles()`

??? func "vertex principal curvature"

    ##### vertex principal curvature

    A 2-symmetric tangent vector field at vertices. The direction corresponds to the first principal direction, and the magnitude corresponds to the ratio of the 1st and 2nd principal curvatures (e.g., if $\kappa_1 \approx \kappa_2$), the magnitude of the field will be near $0$).

    Only valid on triangular meshes.

    - **member:** `VertexData<Vector2> ExtrinsicGeometry::vertexPrincipalCurvatureDirections`
    - **require:** `void ExtrinsicGeometry::requireVertexPrincipalCurvatureDirections()`


## Embedded positions and normals

These quantities depend explicitly on an embedding in 3D space (better known as vertex positions). They are defined for `EmbeddedGeometry` (which is usually instantiated as a `VertexPositionGeometry`). Don't forget, `EmbeddedGeometry` extends the `IntrinsicGeometry` and `ExtrinsicGeometry`, so all of the quantities above are also accessible.


??? func "vertex position"

    ##### vertex position

    Vertex positions in 3D.

    Note that this member is distinct from the `VertexPositionGeometry::inputVertexPositions` field. This field is a derived quantity treated just like any other, it needs to be `require()`'d, etc. It just so happens that this quantity coincides with some input data commonly used to define a geometry (`inputVertexPositions`), but this field does not get any special treatment as such. If you want to update vertex positions on a mesh, you should modify `inputVertexPositions`, not this quantity.

    - **member:** `VertexData<Vector3> EmbeddedGeometry::vertexPositions`
    - **require:** `void EmbeddedGeometry::requireVertexPositions()`
    - **immediate:** `Vector3 EmbeddedGeometry::vertexPosition(Vertex v)`

??? func "halfedge vector"

    ##### halfedge vector

    The vector along each halfedge, that is the vector from the vertex at the tail of the halfedge to the vertex at the tip. 

    Defined as:
    ```cpp
    Vertex vTail = halfedge.vertex();
    Vertex vTip = halfedge.twin().vertex();
    Vector3 halfedgeVector = vertexPositions[vTip] - vertexPositions[vTail]
    ```

    - **member:** `HalfedgeData<Vector3> EmbeddedGeometry::halfedgeVectors`
    - **require:** `void EmbeddedGeometry::requireHalfedgeVectors()`
    - **immediate:** `Vector3 EmbeddedGeometry::halfedgeVector(Halfedge he)`


??? func "face normal"

    ##### face normal

    A normal vector for each face.

    - **member:** `FaceData<Vector3> EmbeddedGeometry::faceNormals`
    - **require:** `void EmbeddedGeometry::requireFaceNormals()`
    - **immediate:** `Vector3 EmbeddedGeometry::faceNormal(Face f)`


??? func "vertex normal"

    ##### vertex normal

    A normal vector for each vertex. Defined as the corner-angle weighted average of incident face normals.

    - **member:** `VertexData<Vector3> EmbeddedGeometry::faceNormals`
    - **require:** `void EmbeddedGeometry::requireFaceNormals()`

??? func "face tangent basis"

    ##### face tangent basis

    An $x$-axis and $y$-axis 3D basis vectors in world space, corresponding to the [intrinsic tangent space](#face-tangent-spaces) for the face. Always orthogonal to the face normal.

    Example:

    ```cpp
 
    HalfedgeMesh& mesh = /* ... */ 
    VertexPositionGeometry& geometry = /* ... */;    
    FaceData<Vector2> myTangentVectorField;
  
    geometry.requireFaceTangentBasis();

    for(Face f : mesh.faces()) {
      Vector2 field = myTangentVectorField[f];

      Vector3 basisX = geometry.faceTangentBasis[f];
      Vector3 basisY = geometry.faceTangentBasis[f];

      Vector3 fieldInWorldCoords = basisX * field.x + basisY * field.y;
    }

    ```

    - **member:** `FaceData<std::array<Vector3,2>> EmbeddedGeometry::faceTangentBasis`
    - **require:** `void EmbeddedGeometry::requireFaceTangentBasis()`


??? func "vertex tangent basis"

    ##### vertex tangent basis

    An $x$-axis and $y$-axis 3D basis vectors in world space, corresponding to the [intrinsic tangent space](#vertex-tangent-spaces) for the vertex. Always orthogonal to the vertex normal.

    Example:

    ```cpp
 
    HalfedgeMesh& mesh = /* ... */ 
    VertexPositionGeometry& geometry = /* ... */;    
    VertexData<Vector2> myTangentVectorField;
  
    geometry.requireFaceTangentBasis();

    for(Vertex v : mesh.vertices()) {
      Vector2 field = myTangentVectorField[v];

      Vector3 basisX = geometry.vertexTangentBasis[v];
      Vector3 basisY = geometry.vertexTangentBasis[v];

      Vector3 fieldInWorldCoords = basisX * field.x + basisY * field.y;
    }

    ```

    - **member:** `VertexData<std::array<Vector3,2>> EmbeddedGeometry::vertexTangentBasis`
    - **require:** `void EmbeddedGeometry::requireVertexTangentBasis()`
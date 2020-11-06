function CreatePolymer(list, shape) {
    var group
    var material;
    if (type == 1) {
        group = group1;
        material = material1

    } else {
        group = group2;
        material = material2;
    }
    var CurvePath = new THREE.CurvePath();
    var Point_last = list[0];
    var ball = ball0.clone();
    ball.translate(Point_last[0], Point_last[1], Point_last[2]);
    var mesh = new THREE.Mesh(ball, material);
    group.add(mesh);
    var type = Point_last[3];

    function out_of_range(point1, point2) {
        for (var j = 0; j < 3; j++) {
            if (Math.abs(point1[j] - point2[j]) > 1) {
                return true;
            }
        }
        return false;
    }


    for (var i = 1; i < list.length; i++) {
        var Point = list[i];

        var ball = ball0.clone();
        ball.translate(Point[0], Point[1], Point[2]);
        var mesh = new THREE.Mesh(ball, material);
        group.add(mesh);

        if (out_of_range(Point_last, Point)) {
            if (CurvePath.curves != 0) {
                var geometry;
                if (type == 1) {
                    geometry = new THREE.TubeBufferGeometry(CurvePath, 20, 0.2, 8, false);
                } else {
                    geometry = new THREE.TubeBufferGeometry(CurvePath, 1, 0.2, 8, false);
                }

                var mesh = new THREE.Mesh(geometry, material); //网格模型对象Mesh
                group.add(mesh);
            }

            CurvePath = new THREE.CurvePath();
            continue;
        }
        let line1 = new THREE.LineCurve3(new THREE.Vector3(Point_last[0], Point_last[1], Point_last[2]),
            new THREE.Vector3(Point[0], Point[1], Point[2]));

        Point_last = Point;
        CurvePath.curves.push(line1);
    }
    if (CurvePath.curves == 0) {
    } else {
        var geometry;
        if (type == 1) {
            geometry = new THREE.TubeBufferGeometry(CurvePath, 20, 0.2, 8, false);
        } else {
            geometry = new THREE.TubeBufferGeometry(CurvePath, 1, 0.2, 8, false);
        }
        var mesh = new THREE.Mesh(geometry, material); //网格模型对象Mesh
        group.add(mesh);
    }
}
function DrawBox(point1, point2) {
    var lines = [];
    var p1 = [0, 0, 0];
    var p2 = [0, 0, 0];
    for (var a = 0; a < 2; a++) {
        for (var b = 0; b < 2; b++) {
            for (var c = 0; c < 3; c++) {
                p1[c] = point1[c];
                p2[c] = point2[c];
                p1[(c + 1) % 3] = a == 0 ? point1[(c + 1) % 3] : point2[(c + 1) % 3];
                p2[(c + 1) % 3] = a == 0 ? point1[(c + 1) % 3] : point2[(c + 1) % 3];
                p1[(c + 2) % 3] = b == 0 ? point1[(c + 2) % 3] : point2[(c + 2) % 3];
                p2[(c + 2) % 3] = b == 0 ? point1[(c + 2) % 3] : point2[(c + 2) % 3];
                let line1 = new THREE.LineCurve3(new THREE.Vector3(p1[0], p1[1], p1[2]), new THREE.Vector3(p2[0], p2[1], p2[2]));
                lines.push(line1);
                console.log(p1, p2);
            }
        }
    }
    var group = new THREE.Group();
    for (var i = 0; i < lines.length; i++) {
        var geometry = new THREE.TubeBufferGeometry(lines[i], 1, 0.1, 4, false);
        var material = new THREE.MeshBasicMaterial({
            color: 0x110011,
            side: THREE.DoubleSide
        });
        var mesh = new THREE.Mesh(geometry, material); //网格模型对象Mesh
        group.add(mesh);

    }

    scene.add(group);

}
var insMesh;
function ballls() {

    var ball0 = new THREE.SphereGeometry(0.15, 5, 5);
    let line1 = new THREE.LineCurve3(new THREE.Vector3(0, 0, 0),
        new THREE.Vector3(1, 0, 0));
    var geometry = new THREE.TubeGeometry(line1, 1, 0.15, 8, false);

    var ball1 = new THREE.SphereGeometry(0.15, 5, 5);
    geometry.merge(ball1);
    geometry.merge(ball0);

    insMesh = new THREE.InstancedMesh(geometry, material2, 24 * 24 * 24);

    //修改位置
    var sq2 = Math.sqrt(2);
    var sq3 = Math.sqrt(3);
    let transform = new THREE.Object3D();
    direction = [1, 1, 1];

    function add(position, direction) {
        if (direction == [-1, -1, -1]) {
            transform.scale.set(sq3, 1, 1);
            transform.rotateY(3 * Math.PI / 4);
            transform.rotateZ(Math.PI / 4);
            transform.position.set(position[0], position[1], position[2])
            transform.updateMatrix();
            console.log(transform.matrix)
            insMesh.setMatrixAt(index, transform.matrix);
            transform.rotateZ(-Math.PI / 4);
            transform.rotateY(-3 * Math.PI / 4);
        }
        else if (direction == [-1, -1, 0]) {

            transform.scale.set(sq2, 1, 1);

            transform.rotateZ(-3 * Math.PI / 4);
            transform.position.set(position[0], position[1], position[2])
            transform.updateMatrix();
            console.log(transform.matrix)
            insMesh.setMatrixAt(index, transform.matrix);
            transform.rotateY(Math.PI);
        }
        else if (direction == [-1, -1, 1]) {
            transform.scale.set(sq3, 1, 1);

            transform.rotateY(-Math.PI / 4);
            transform.rotateZ(-3 * Math.PI / 4);
            transform.position.set(position[0], position[1], position[2])
            transform.updateMatrix();
            console.log(transform.matrix)
            insMesh.setMatrixAt(index, transform.matrix);
            transform.rotateZ(3 * Math.PI / 4);
            transform.rotateY(Math.PI / 4);
        }
        else if (direction == [-1, 0, -1]) {

            transform.scale.set(sq2, 1, 1);
            transform.rotateY(3 * Math.PI / 4);
            transform.position.set(position[0], position[1], position[2])
            transform.updateMatrix();
            console.log(transform.matrix)
            insMesh.setMatrixAt(index, transform.matrix);
            transform.rotateY(-3 * Math.PI / 2);
        }
        else if (direction == [-1, 0, 0]) {
            transform.rotateY(Math.PI / 2);
            transform.position.set(position[0], position[1], position[2])
            transform.updateMatrix();
            console.log(transform.matrix)
            insMesh.setMatrixAt(index, transform.matrix);
            transform.rotateY(-Math.PI / 2);

        }
        else if (direction == [-1, 0, 1]) {
            transform.scale.set(sq2, 1, 1);
            transform.rotateY(-3 * Math.PI / 4);
            transform.position.set(position[0], position[1], position[2])
            transform.updateMatrix();
            insMesh.setMatrixAt(index, transform.matrix);
            transform.rotateZ(3 * Math.PI / 4);
        }
        else if (direction == [-1, 1, -1]) {

            transform.scale.set(sq3, 1, 1);

            transform.rotateY(3 * Math.PI / 4);
            transform.rotateZ(-Math.PI / 4);
            transform.position.set(position[0], position[1], position[2])
            transform.updateMatrix();
            console.log(transform.matrix)
            insMesh.setMatrixAt(index, transform.matrix);
            transform.rotateZ(Math.PI / 4);
            transform.rotateY(-3 * Math.PI / 4);

        }
        else if (direction == [-1, 1, 0]) {/////
            transform.scale.set(sq2, 1, 1);
            transform.rotateY(Math.PI / 2);
            transform.rotateZ(3 * Math.PI / 4);
            transform.position.set(position[0], position[1], position[2])
            transform.updateMatrix();
            insMesh.setMatrixAt(index, transform.matrix);
            transform.rotateZ(-3 * Math.PI / 4);
            transform.rotateY(-Math.PI / 2);

        }
        else if (direction == [-1, 1, 1]) {

            transform.scale.set(sq3, 1, 1);

            transform.rotateY(-3 * Math.PI / 4);
            transform.rotateZ(-Math.PI / 4);
            transform.position.set()//TODO
            transform.position.set(position[0], position[1], position[2])
            console.log(transform.matrix)
            insMesh.setMatrixAt(index, transform.matrix);
            transform.rotateZ(Math.PI / 4);
            transform.rotateY(3 * Math.PI / 4);


        }
        else if (direction == [0, -1, -1]) {///
            transform.scale.set(sq2, 1, 1);
            transform.rotateY(Math.PI / 2);
            transform.rotateZ(3 * Math.PI / 4);
            transform.position.set()//TODO
            transform.position.set(position[0], position[1], position[2])
            insMesh.setMatrixAt(index, transform.matrix);
            transform.rotateZ(-3 * Math.PI / 4);
            transform.rotateY(-Math.PI / 2);

        } else if (direction == [0, -1, 0]) {
            transform.rotateZ(-Math.PI / 2);
            transform.position.set()//TODO
            transform.position.set(position[0], position[1], position[2])
            insMesh.setMatrixAt(index, transform.matrix);
            transform.rotateZ(Math.PI / 2);

        }
        else if (direction == [0, -1, 1]) {
            transform.scale.set(sq2, 1, 1);
            transform.rotateY(-3 * Math.PI / 4);
            transform.position.set(position[0], position[1], position[2])
            transform.updateMatrix();
            insMesh.setMatrixAt(index, transform.matrix);
            transform.rotateZ(-3 * Math.PI / 4);
            transform.rotateY(3 * Math.PI / 4);

        }
        else if (direction == [0, 0, -1]) {
            transform.rotateY(Math.PI / 2);
            transform.position.set(position[0], position[1], position[2])
            transform.updateMatrix();
            insMesh.setMatrixAt(index, transform.matrix);
            transform.rotateY(-Math.PI / 2);

        }
        else if (direction == [0, 0, 1]) {
            transform.rotateY(-Math.PI / 2);
            transform.position.set(position[0], position[1], position[2])
            transform.updateMatrix();
            insMesh.setMatrixAt(index, transform.matrix);
            transform.rotateY(Math.PI / 2);

        }
        else if (direction == [0, 1, -1]) {///

            transform.scale.set(sq2, 1, 1);
            transform.rotateY(Math.PI / 2);
            transform.rotateX(Math.PI / 4);
            transform.position.set(position[0], position[1], position[2])
            transform.updateMatrix();
            insMesh.setMatrixAt(index, transform.matrix);
            transform.rotateZ(-Math.PI / 4);
            transform.rotateY(-Math.PI / 2);

        }
        else if (direction == [0, 1, 0]) {
            transform.rotateZ(Math.PI / 2);
            transform.position.set(position[0], position[1], position[2])
            transform.updateMatrix();
            insMesh.setMatrixAt(index, transform.matrix);
            transform.rotateZ(-Math.PI / 2);

        }
        else if (direction == [0, 1, 1]) {

            transform.scale.set(sq2, 1, 1);
            transform.rotateY(-Math.PI / 2);
            transform.rotateX(-Math.PI / 4);
            transform.position.set(position[0], position[1], position[2])
            transform.updateMatrix();
            insMesh.setMatrixAt(index, transform.matrix);
            transform.rotateZ(Math.PI / 4);
            transform.rotateY(Math.PI / 2);

        }

        else if (direction == [1, -1, -1]) {
            transform.scale.set(sq2, 1, 1);
            transform.rotateY(Math.PI / 4);
            transform.rotateZ(Math.PI / 4);
            transform.position.set(position[0], position[1], position[2])
            transform.updateMatrix();
            insMesh.setMatrixAt(index, transform.matrix);
            transform.rotateZ(-Math.PI / 4);
            transform.rotateY(-Math.PI / 4);

        }

        else if (direction == [1, -1, 0]) {///
            transform.scale.set(sq2, 1, 1);
            transform.rotateZ(- Math.PI / 4);
            transform.position.set()//TODO
            transform.updateMatrix();
            insMesh.setMatrixAt(index, transform.matrix);
            transform.rotateZ(-Math.PI / 4);

        } else if (direction == [1, -1, 1]) {

            transform.scale.set(sq2, 1, 1);
            transform.rotateY(-Math.PI / 4);
            transform.rotateZ(-Math.PI / 4);
            transform.position.set(position[0], position[1], position[2])
            transform.updateMatrix();
            insMesh.setMatrixAt(index, transform.matrix);
            transform.rotateZ(Math.PI / 4);
            transform.rotateY(Math.PI / 4);
            transform.scale.set(1 / sq2, 1, 1);

        }
        else if (direction == [1, 0, -1]) {////////
            transform.scale.set(sq2, 1, 1);
            transform.rotateY(4 * Math.PI / 4);
            transform.position.set(position[0], position[1], position[2])
            transform.updateMatrix();
            insMesh.setMatrixAt(index, transform.matrix);
            transform.rotateY(-Math.PI / 4);

        }
        else if (direction == [1, 0, 0]) {


            transform.position.set(position[0], position[1], position[2])
            transform.updateMatrix();
            insMesh.setMatrixAt(index, transform.matrix);

        }
        else if (direction == [1, 0, 1]) {
            transform.scale.set(sq2, 1, 1);
            transform.rotateY(-Math.PI / 4);
            transform.position.set(position[0], position[1], position[2])
            transform.updateMatrix();
            insMesh.setMatrixAt(index, transform.matrix);
            transform.rotateY(Math.PI / 4);
        }
        else if (direction == [1, 1, -1]) {
            transform.scale.set(sq3, 1, 1);
            transform.rotateY(Math.PI / 4);
            transform.rotateZ(Math.PI / 4);
            transform.position.set(position[0], position[1], position[2])
            transform.updateMatrix();
            insMesh.setMatrixAt(index, transform.matrix);
            transform.rotateZ(-Math.PI / 4);
            transform.rotateY(-Math.PI / 4);


        }
        else if (direction == [1, 1, 0]) {
            transform.scale.set(sq2, 1, 1);
            transform.rotateZ(Math.PI / 4);
            transform.position.set(position[0], position[1], position[2])
            transform.updateMatrix();
            insMesh.setMatrixAt(index, transform.matrix);
            transform.rotateZ(-Math.PI / 4);
        }
        else if (direction == [1, 1, 1]) {
            transform.scale.set(sq2, 1, 1);
            transform.rotateZ(Math.PI / 4);
            transform.position.set(position[0], position[1], position[2])
            transform.updateMatrix();
            insMesh.setMatrixAt(index, transform.matrix);
            transform.rotateZ(-Math.PI / 4);

        }
        index++;
    }

    scene.add(insMesh);

}
ballls();

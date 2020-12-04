const fs = require('fs');

const rand_1_and_20 = () => Math.floor(Math.random() * 201) - 100;
const rand_1_and_5 = () => Math.floor(Math.random() * 51) - 25;
//
//const rand_1_and_20 = () => Math.random();
//const rand_1_and_5 = () => Math.random();

// CREATE TRAINING DATA
function generateTrainData() {
  const stream_train = fs.createWriteStream("/Users/rafiq/xcode_p/xor/xor/quad_train.txt");
  stream_train.write("10000 2 2\n")
  for (let i = 0; i < 10000; i++) {

    const r1 = rand_1_and_20();
    const r2 = rand_1_and_20();

    const b = (r1 + r2);
    const c = r1 * r2;
    let small_root, big_root;

    if (r1 < r2) {
      small_root = r1;
      big_root = r2;
    } else if (r1 > r2) {
      small_root = r2;
      big_root = r1
    } else {
      small_root = r1;
      big_root = small_root;
    }

    stream_train.write(
      `${parseFloat(b.toFixed(6))} ${parseFloat(c.toFixed(6))}\n${parseFloat(small_root.toFixed(6))} ${parseFloat(big_root.toFixed(6))}\n`
    )
  }
}

// CREATE UNSEEN TESTING DATA
function generateTestData() {
  const steam_test = fs.createWriteStream("/Users/rafiq/xcode_p/xor/xor/quad_test.txt");
  steam_test.write("500 2 2\n")

  for (let i = 0; i < 500; i++) {
    const r1 = rand_1_and_5();
    const r2 = rand_1_and_5();

    const b = (r1 + r2);
    const c = r1 * r2;
    let small_root, big_root;

    if (r1 < r2) {
      small_root = r1;
      big_root = r2;
    } else if (r1 > r2) {
      small_root = r2;
      big_root = r1
    } else {
      small_root = r1;
      big_root = small_root;
    }

    steam_test.write(
      `${parseFloat(b.toFixed(6))} ${parseFloat(c.toFixed(6))}\n${parseFloat(small_root.toFixed(6))} ${parseFloat(big_root.toFixed(6))}\n`
    )
  }
}

function run() {
  try {
    generateTrainData();
    generateTestData();
  } catch (e) {
    console.log("Error Message: ", e.message)
    console.log("ERROR: ", e)
  }

}

run();



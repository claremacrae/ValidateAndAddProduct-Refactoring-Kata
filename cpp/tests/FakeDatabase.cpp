#include "FakeDatabase.h"

int FakeDatabase::storeProduct(Product *aProduct) {
    product_ = aProduct;
    return 0;
}

Product *FakeDatabase::product() {
    return product_;
}

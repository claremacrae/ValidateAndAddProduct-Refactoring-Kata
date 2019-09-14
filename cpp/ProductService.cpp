#include "ProductService.h"
#include "ProductFormData.h"
#include "Product.h"

ProductService::ProductService(IDatabase * database) : db(database) {

}

Product* createProduct(ProductFormData *productData) {
    auto product = new Product(productData->name);
    product->type = ("Unknown");

    if (productData->name.empty())
    {
        return product;
    }

    if (productData->type.empty())
    {
        return product;
    }

    if ("Eyeshadow" == (productData->type) || "Mascara" == (productData->type))
    {
        product->type = (productData->type);
        product->family = ProductFamily::EYES;
        if ("Eyeshadow" == (productData->type) && product->name.find("Queen") != std::string::npos)
        {
            product->range = (ProductRange::QUEEN);
        }
    }

    product->range = (ProductRange::BUDGET);
    if (productData->packagingRecyclable)
    {
        product->range = (ProductRange::PROFESSIONAL);
    }

    if ("Foundation" == (productData->type))
    {
        if (productData->suggestedPrice > 10)
        {
            product->range = (ProductRange::PROFESSIONAL);
        }
    }

    if ("Lipstick" == (productData->type))
    {
        product->type = (productData->type);
        product->family = (ProductFamily::LIPS);
        if (productData->suggestedPrice > 10)
        {
            product->range = (ProductRange::PROFESSIONAL);
        }

        if (productData->suggestedPrice > 20)
        {
            product->range = (ProductRange::QUEEN);
        }
    }

    if ("Mascara" == (productData->type))
    {
        product->family = (ProductFamily::LASHES);
        if (productData->suggestedPrice > 15)
        {
            product->range = (ProductRange::PROFESSIONAL);
        }

        if (productData->suggestedPrice > 25 && productData->packagingRecyclable)
        {
            product->range = (ProductRange::QUEEN);
        }
    }

    product->weight = (productData->weight);

    if ("Blusher" == (productData->type) || "Foundation" == (productData->type))
    {
        product->type = (productData->type);
        product->family = (ProductFamily::SKIN);
    }

    return product;

}
Response validate(const Product* const product, ProductFormData *productData) {
    if (productData->name.empty())
    {
        return Response(0, Response::MISSING_DATA_ERROR, "Missing Name");
    }

    if (productData->type.empty())
    {
        return Response(0, Response::MISSING_DATA_ERROR, "Missing Type");
    }

    if ("Lipstick" == (productData->type))
    {
        if (productData->suggestedPrice > 20)
        {
            if (productData->weight > 0 && productData->weight < 10)
            {
                return Response(0, Response::FAILED_QUALITY_CHECK, "Error - failed quality check for Queen Range");
            }
        }
    }

    if (productData->weight < 0)
    {
        return Response(0, Response::WEIGHT_ERROR, "Weight error");
    }

    if ("Blusher" == (productData->type) || "Foundation" == (productData->type))
    {
        if ("Blusher" == (productData->type) && productData->weight > 10)
        {
            return Response(0, Response::WEIGHT_ERROR, "Error - weight too high");
        }
    }

    if (!productData->packagingRecyclable && product->range == ProductRange::QUEEN)
    {
        return Response(0, Response::FAILED_QUALITY_CHECK, "Error - failed quality check for Queen Range");
    }

    if ("Unknown" == (product->type))
    {
        return Response(0, Response::FAILED_QUALITY_CHECK, "Unknown product type " + productData->type);
    }

    return Response(0, Response::OK, "Product Successfully Added");

}
Response ProductService::validateAndAdd(ProductFormData *productData) {
    auto product = createProduct(productData);
    auto response = validate(product, productData);
    if ( response.statusCode == Response::OK)
    {
        response.productId = db->storeProduct(product);
    }

    return response;
}

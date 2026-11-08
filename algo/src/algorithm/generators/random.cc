#include "rossignol/algorithm/generators/random.hh"

#include <random>

#include "rossignol/algorithm/generators/generate_image.hh"
#include "rossignol/utils/typing/any_of.hh"


namespace {

class random_generator_base {
public:
    random_generator_base(): _rd(), _gen(_rd()) {}

private:
    std::random_device _rd;

protected:
    std::mt19937 _gen;
};


class bernoulli_generator: random_generator_base {
public:
    bernoulli_generator(double probability): _distrib(probability) {}

    bool operator()() {
        return _distrib(_gen);
    }

private:
    std::bernoulli_distribution _distrib;
};


template<rol::random_method Method>
class u8_generator;

template<>
class u8_generator<rol::random_method::uniform>: public random_generator_base {
public:
    u8_generator(): _distrib() {}

    uint8_t operator()() {
        return _distrib(_gen);
    }

private:
    std::uniform_int_distribution<uint8_t> _distrib;
};

template<>
class u8_generator<rol::random_method::gaussian>: public random_generator_base {
public:
    u8_generator(): _distrib(127.0, 45.0) {}

    uint8_t operator()() {
        const double res = _distrib(_gen);
        if(res <= 0.0) {
            return 0;
        }
        if(res >= 255.0) {
            return 255;
        }
        return static_cast<uint8_t>(std::lround(res));
    }

private:
    std::normal_distribution<double> _distrib;
};


template<rol::random_method Method>
class coefficient_generator;

template<>
class coefficient_generator<rol::random_method::uniform>: public random_generator_base {
public:
    coefficient_generator(): _distrib(0.0, 1.0) {}

    double operator()() {
        return _distrib(_gen);
    }

private:
    std::uniform_real_distribution<double> _distrib;
};

template<>
class coefficient_generator<rol::random_method::gaussian>: public random_generator_base {
public:
    coefficient_generator(): _distrib(0.5, 0.125) {}

    double operator()() {
        const double res = _distrib(_gen);
        if(res <= 0.0) {
            return 0.0;
        }
        if(res >= 1.0) {
            return 1.0;
        }
        return res;
    }

private:
    std::normal_distribution<double> _distrib;
};

} // anonymous namespace


namespace rol::algo {

binary_image random_binary_image(const math::vec2u& size, double bernoulli_probability) {
    ::bernoulli_generator gen(bernoulli_probability);
    return generate_image<bool>(size, gen);
}


template<random_method Method>
layer random_layer(const math::vec2u& size) {
    ::u8_generator<Method> gen;
    return generate_image<uint8_t>(size, gen);
}

template layer random_layer<rol::random_method::uniform>(const math::vec2u& size);
template layer random_layer<rol::random_method::gaussian>(const math::vec2u& size);


template<random_method Method>
coefficient_plane random_coefficient_plane(const math::vec2u& size) {
    ::coefficient_generator<Method> gen;
    return generate_image<double>(size, gen);
}

template coefficient_plane random_coefficient_plane<rol::random_method::uniform>(const math::vec2u& size);
template coefficient_plane random_coefficient_plane<rol::random_method::gaussian>(const math::vec2u& size);


template<random_method Method>
greyscale_image random_greyscale_image(const math::vec2u& size) {
    ::u8_generator<Method> gen;
    return generate_image<greyscalea>(size, [&gen]() {
        return greyscalea {
            .grey = gen(),
            .a = 255
        };
    });
}

template greyscale_image random_greyscale_image<rol::random_method::uniform>(const math::vec2u& size);
template greyscale_image random_greyscale_image<rol::random_method::gaussian>(const math::vec2u& size);


template<random_method Method>
rgb_image random_rgb_image(const math::vec2u& size) {
    ::u8_generator<Method> gen;
    return generate_image<rgba>(size, [&gen]() {
        return rgba {
            .r = gen(),
            .g = gen(),
            .b = gen(),
            .a = 255
        };
    });
}

template rgb_image random_rgb_image<rol::random_method::uniform>(const math::vec2u& size);
template rgb_image random_rgb_image<rol::random_method::gaussian>(const math::vec2u& size);

} // namespace rol::algo

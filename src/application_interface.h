#pragma once

#include <iostream>
#include "chipmunk/chipmunk.h"
#include "world.h"

SDL_Rect markus_pos = {304, 226, 32, 32};
SDL_Texture* markus = nullptr;
SDL_Texture* heads[7] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
SDL_Texture* melone = nullptr;
SDL_Texture* arm = nullptr;

// World stuff
world w;

void drawt(SDL_Texture* t, SDL_Rect* r) {
    SDL_RenderCopy(engine_data->sdl2_data.renderer.handle, t, nullptr, r);
}

long double radiansaresuperior(long double a) {
    return a * 180.0 / 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196442881097566593344612847564823378678316527120190914564856692346034861045432664821339360726024914127372458700660631558817488152092096282925409171536436789259036001133053054882046652138414695194151160943305727036575959195309218611738193261179310511854807446237996274956735188575272489122793818301194912983367336244065664308602139494639522473719070217986094370277053921717629317675238467481846766940513200056812714526356082778577134275778960917363717872146844090122495343014654958537105079227968925892354201995611212902196086403441815981362977477130996051870721134999999837297804995105973173281609631859502445945534690830264252230825334468503526193118817101000313783875288658753320838142061717766914730359825349042875546873115956286388235378759375195778185778053217122680661300192787661119590921642019893809525720106548586327886593615338182796823030195203530185296899577362259941389124972177528347913151557485724245415069595082953311686172785588907509838175463746493931925506040092770167113900984882401285836160356370766010471018194295559619894676783744944825537977472684710404753464620804668425906949129331367702898915210475216205696602405803815019351125338243003558764024749647326391419927260426992279678235478163600934172164121992458631503028618297455570674983850549458858692699569092721079750930295532116534498720275596023648066549911988183479775356636980742654252786255181841757467289097777279380008164706001614524919217321721477235014144197356854816136115735255213347574184946843852332390739414333454776241686251898356948556209921922218427255025425688767179049460165346680498862723279178608578438382796797668145410095388378636095068006422512520511739298489608412848862694560424196528502221066118630674427862203919494504712371378696095636437191728746776465757396241389086583264599581339047802759009946576407895126946839835259570982582262052248940772671947826848260147699090264013639443745530506820349625245174939965143142980919065925093722169646151570985838741059788595977297549893016175392846813826868386894277415599185592524595395943104997252468084598727364469584865383673622262609912460805124388439045124413654976278079771569143599770012961608944169486855584840635342207222582848864815845602850601684273945226746767889525213852254995466672782398645659611635488623057745649803559363456817432411251507606947945109659;
}

void drawtr(SDL_Texture* t, SDL_Rect* r, double a) {
    SDL_RenderCopyEx(engine_data->sdl2_data.renderer.handle, t, nullptr, r, radiansaresuperior(a), nullptr, static_cast<SDL_RendererFlip>(0));
}

void drawtrrr(SDL_Texture* t, SDL_Rect* r, double a) {
    SDL_Point rr = {0, 0};
    SDL_RenderCopyEx(engine_data->sdl2_data.renderer.handle, t, nullptr, r, radiansaresuperior(a), &rr, static_cast<SDL_RendererFlip>(0));
}

#include "database.h"

struct ImageLoader {
    using type = SDL_Texture;

    static type* load( const std::string& s ) {
        SDL_Texture* t = IMG_LoadTexture( engine_data->sdl2_data.renderer.handle, s.c_str() );
        if ( t == nullptr ) {
            std::string msg = std::string( "OMGFUCKYOU CANNOT LOAD THOSE FINE TAPESTRIES: " ) + s;
            crash( msg.c_str() ); // crash kill and destroy
        }
        return t;
    }

    static void unload( type* t ) {
        SDL_DestroyTexture(t);
    }

    static uint64_t size() { return 0; } // unused for now...
};

database<ImageLoader> imdb;

SDL_Texture* load_or_die(const char* imagefilename) {
    return imdb.add( std::string( imagefilename ) );
}

void update_application_logic() {
    updatew(w);
}

void draw_arm_thing(cpBody* body, cpShape* shape, double camx, double camy) {
    cpVect pos = cpBodyGetPosition(body);
    cpFloat radius = cpPolyShapeGetRadius(shape);
    cpVect rot = cpBodyGetRotation(body);
    double t = atan2(rot.y, rot.x);
    for (int i = 1; i <= cpPolyShapeGetCount(shape); ++i) {
        cpVect from = cpPolyShapeGetVert(shape, i - 1);
        int j = i;
        if (j == cpPolyShapeGetCount(shape)) {
            j = 0;
        }
        cpVect to = cpPolyShapeGetVert(shape, j);
        double fromt = atan2(from.y, from.x);
        double fromr = sqrt(from.y*from.y + from.x*from.x);
        double tot = atan2(to.y, to.x);
        double tor = sqrt(to.y*to.y + to.x*to.x);

        double fromx = fromr * cos(t + fromt);
        double fromy = fromr * sin(t + fromt);
        double tox = tor * cos(t + tot);
        double toy = tor * sin(t + tot);

        double tt = atan2(fromy - toy, fromx - tox);

        SDL_Rect r;
        r.x = pos.x + fromx - camx + 320;
        r.y = pos.y + fromy - camy + 240;
        r.w = 32;
        r.h = 130;
        drawtrrr(arm, &r, tt + (3.14159265358979 / 2));
    }
}

void draw_application_view() {
    drawworld_debug(w);
    cpVect playerpos = cpBodyGetPosition(w.players.data[0].body);
    unsigned int c = 0;
    for (auto& house : w.houses) {
        draw_arm_thing(house.body, house.shape, playerpos.x, playerpos.y);
    }
    for (auto& annoying_friend : w.annoying_friends) {
        cpVect pos = cpBodyGetPosition(annoying_friend.body);
        cpVect rot = cpBodyGetRotation(annoying_friend.body);
        double a = atan2(rot.y, rot.x);
        SDL_Rect r;
        r.x = pos.x - playerpos.x + 320 - 8;
        r.y = pos.y - playerpos.y + 240 - 15;
        r.w = 32;
        r.h = 40;
        drawtr(heads[c++], &r, a);
        if (c > 7) {
            c = 0;
        }
    }
    for (auto& melon : w.melons) {
        cpVect pos = cpBodyGetPosition(melon.body);
        cpVect rot = cpBodyGetRotation(melon.body);
        double a = atan2(rot.y, rot.x);
        SDL_Rect r;
        r.x = pos.x - playerpos.x + 320 - 8;
        r.y = pos.y - playerpos.y + 240 - 15;
        r.w = 32;
        r.h = 32;
        drawtr(melone, &r, a);
    }
    drawt(markus, &markus_pos);
}

void init_application() {
    markus = load_or_die("res/image/markus/head.png");
    heads[0] = load_or_die("res/image/people/head1.png");
    heads[1] = load_or_die("res/image/people/head2.png");
    heads[2] = load_or_die("res/image/people/head3.png");
    heads[3] = load_or_die("res/image/people/head4.png");
    heads[4] = load_or_die("res/image/people/head5.png");
    heads[5] = load_or_die("res/image/people/head6.png");
    heads[6] = load_or_die("res/image/people/head7.png");
    melone = load_or_die("res/image/melon.png");
    arm = load_or_die("res/image/arm.png");
    loadworld(w, "res/maps/example.csv");
}

void close_application() {
    destroytheworld(w);
    SDL_DestroyTexture(markus);
    SDL_DestroyTexture(heads[0]);
    SDL_DestroyTexture(heads[1]);
    SDL_DestroyTexture(heads[2]);
    SDL_DestroyTexture(heads[3]);
    SDL_DestroyTexture(heads[4]);
    SDL_DestroyTexture(heads[5]);
    SDL_DestroyTexture(heads[6]);
    SDL_DestroyTexture(melone);
    SDL_DestroyTexture(arm);
}

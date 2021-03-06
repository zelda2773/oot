#include <ultra64.h>
#include <global.h>

typedef enum {
    /* 0x00 */ A_OBJ_BLOCK_SMALL,
    /* 0x01 */ A_OBJ_BLOCK_LARGE,
    /* 0x02 */ A_OBJ_BLOCK_HUGE,
    /* 0x03 */ A_OBJ_BLOCK_SMALL_ROT,
    /* 0x04 */ A_OBJ_BLOCK_LARGE_ROT,
    /* 0x05 */ A_OBJ_CUBE_SMALL,
    /* 0x06 */ A_OBJ_UNKNOWN_6,
    /* 0x07 */ A_OBJ_GRASS_CLUMP,
    /* 0x08 */ A_OBJ_TREE_STUMP,
    /* 0x09 */ A_OBJ_SIGNPOST_OBLONG,
    /* 0x0A */ A_OBJ_SIGNPOST_ARROW,
    /* 0x0B */ A_OBJ_KNOB
} AObjType;

typedef struct {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x164 */ ActorFunc actionFunc;
    /* 0x168 */ s32 unk_168;
    /* 0x16C */ s16 textId;
    /* 0x16E */ s16 unk_16E;
    /* 0x170 */ s16 unk_170;
    /* 0x172 */ s16 unk_172;
    /* 0x174 */ s16 unk_174;
    /* 0x178 */ f32 unk_178;
    /* 0x17C */ ColliderCylinder collider;
} EnAObj; // size = 0x1C8

#define FLAGS 0x00000010

void func_8001D204(EnAObj* this, GlobalContext* globalCtx);
void func_8001D25C(EnAObj* this, GlobalContext* globalCtx);
void func_8001D360(EnAObj* this, GlobalContext* globalCtx);
void func_8001D4A8(EnAObj* this, GlobalContext* globalCtx);
void func_8001D608(EnAObj* this, GlobalContext* globalCtx);

void func_8001D234(EnAObj* this, s16 params);
void func_8001D310(EnAObj* this, s16 params);
void func_8001D480(EnAObj* this, s16 params);
void func_8001D5C8(EnAObj* this, s16 params);

void EnAObj_Init(EnAObj* this, GlobalContext* globalCtx);
void EnAObj_Destroy(EnAObj* this, GlobalContext* globalCtx);
void EnAObj_Update(EnAObj* this, GlobalContext* globalCtx);
void EnAObj_Draw(EnAObj* this, GlobalContext* globalCtx);

const ActorInit En_A_Obj_InitVars = {
    ACTOR_EN_A_OBJ,
    ACTORTYPE_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnAObj),
    (ActorFunc)EnAObj_Init,
    (ActorFunc)EnAObj_Destroy,
    (ActorFunc)EnAObj_Update,
    (ActorFunc)EnAObj_Draw,
};

// TODO: Define this section of .data here and rename the symbols
extern ColliderCylinderInit D_80115440;
extern u32 D_8011546C[];
extern u32 D_80115484[];

void EnAObj_SetupAction(EnAObj* this, ActorFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void EnAObj_Init(EnAObj* this, GlobalContext* globalCtx) {
    u32 sp34;
    s32 pad;
    EnAObj* this2 = this;
    f32 sp28;

    sp34 = 0;
    sp28 = 6.0f;

    this->textId = (this->dyna.actor.params >> 8) & 0xFF;
    this->dyna.actor.params &= 0xFF;

    switch (this->dyna.actor.params) {
        case A_OBJ_BLOCK_SMALL:
            Actor_SetScale(&this->dyna.actor, 0.025f);
            break;
        case A_OBJ_BLOCK_LARGE:
            Actor_SetScale(&this->dyna.actor, 0.05f);
            break;
        case A_OBJ_BLOCK_HUGE:
        case A_OBJ_CUBE_SMALL:
        case A_OBJ_UNKNOWN_6:
            Actor_SetScale(&this->dyna.actor, 0.1f);
            break;
        case A_OBJ_BLOCK_SMALL_ROT:
            Actor_SetScale(&this->dyna.actor, 0.005f);
            break;
        case A_OBJ_BLOCK_LARGE_ROT:
        default:
            Actor_SetScale(&this->dyna.actor, 0.01f);
            break;
    }

    if (this->dyna.actor.params >= 9) {
        sp28 = 12.0f;
    }

    ActorShape_Init(&this->dyna.actor.shape, 0.0f, ActorShadow_DrawFunc_Circle, sp28);

    this->dyna.actor.posRot2.pos = this->dyna.actor.posRot.pos;
    this->dyna.dynaPolyId = -1;
    this->dyna.unk_160 = 0;
    this->dyna.unk_15C = 0;
    this->dyna.actor.unk_FC = 1200.0f;
    this->dyna.actor.unk_F8 = 200.0f;

    switch (this->dyna.actor.params) {
        case A_OBJ_BLOCK_LARGE:
        case A_OBJ_BLOCK_HUGE:
            this2->dyna.dynaPolyId = 1;
            Actor_ChangeType(globalCtx, &globalCtx->actorCtx, &this->dyna.actor, ACTORTYPE_BG);
            func_8001D5C8(this2, this->dyna.actor.params);
            break;
        case A_OBJ_BLOCK_SMALL_ROT:
        case A_OBJ_BLOCK_LARGE_ROT:
            this2->dyna.dynaPolyId = 3;
            Actor_ChangeType(globalCtx, &globalCtx->actorCtx, &this->dyna.actor, ACTORTYPE_BG);
            func_8001D310(this2, this->dyna.actor.params);
            break;
        case A_OBJ_UNKNOWN_6:
            // clang-format off
            this->dyna.actor.flags |= 0x1; this2->dyna.dynaPolyId = 5; this2->unk_178 = 10.0f;
            // clang-format on
            this->dyna.actor.gravity = -2.0f;
            func_8001D234(this2, this->dyna.actor.params);
            break;
        case A_OBJ_GRASS_CLUMP:
        case A_OBJ_TREE_STUMP:
            this2->dyna.dynaPolyId = 0;
            func_8001D234(this2, this->dyna.actor.params);
            break;
        case A_OBJ_SIGNPOST_OBLONG:
        case A_OBJ_SIGNPOST_ARROW:
            this->dyna.actor.textId = (this2->textId & 0xFF) | 0x300;
            // clang-format off
            this->dyna.actor.flags |= 0x1 | 0x8; this->dyna.actor.unk_4C = 500.0f;
            // clang-format on
            this2->unk_178 = 45.0f;
            func_8001D234(this2, this->dyna.actor.params);
            Collider_InitCylinder(globalCtx, &this2->collider);
            Collider_SetCylinder(globalCtx, &this2->collider, &this->dyna.actor, &D_80115440);
            this->dyna.actor.colChkInfo.mass = 0xFF;
            this->dyna.actor.unk_1F = 0;
            break;
        case A_OBJ_KNOB:
            this->dyna.actor.gravity = -1.5f;
            func_8001D480(this2, this->dyna.actor.params);
            break;
        default:
            this->dyna.actor.gravity = -2.0f;
            func_8001D234(this2, this->dyna.actor.params);
            break;
    }

    if (this->dyna.actor.params < 5) {
        this->dyna.actor.colChkInfo.mass = 0xFF;
    }

    if (this->dyna.dynaPolyId != -1) {
        DynaPolyInfo_Alloc(D_8011546C[this->dyna.dynaPolyId], &sp34);
        this->dyna.dynaPolyId = DynaPolyInfo_RegisterActor(globalCtx, &globalCtx->colCtx.dyna, &this->dyna.actor, sp34);
    }
}

void EnAObj_Destroy(EnAObj* this, GlobalContext* globalCtx) {
    ColliderCylinder* collider = &this->collider;

    DynaPolyInfo_Free(globalCtx, &globalCtx->colCtx.dyna, this->dyna.dynaPolyId);

    switch (this->dyna.actor.params) {
        case A_OBJ_SIGNPOST_OBLONG:
        case A_OBJ_SIGNPOST_ARROW:
            Collider_DestroyCylinder(globalCtx, collider);
    }
}

void func_8001D204(EnAObj* this, GlobalContext* globalCtx) {
    if (func_8002F334(&this->dyna.actor, globalCtx)) {
        func_8001D234(this, this->dyna.actor.params);
    }
}

void func_8001D234(EnAObj* this, s16 params) {
    EnAObj_SetupAction(this, (ActorFunc)func_8001D25C);
}

void func_8001D25C(EnAObj* this, GlobalContext* globalCtx) {
    s16 var;

    if (this->dyna.actor.textId != 0) {
        var = this->dyna.actor.rotTowardsLinkY - this->dyna.actor.shape.rot.y;
        if ((ABS(var) < 0x2800) || ((this->dyna.actor.params == 0xA) && (ABS(var) > 0x5800))) {
            if (func_8002F194(&this->dyna.actor, globalCtx)) {
                EnAObj_SetupAction(this, (ActorFunc)func_8001D204);
            } else {
                func_8002F2F4(&this->dyna.actor, globalCtx);
            }
        }
    }
}

void func_8001D310(EnAObj* this, s16 params) {
    this->unk_16E = 0;
    this->unk_168 = 10;
    this->dyna.actor.posRot.rot.y = 0;
    this->dyna.actor.shape.rot = this->dyna.actor.posRot.rot;
    EnAObj_SetupAction(this, (ActorFunc)func_8001D360);
}

void func_8001D360(EnAObj* this, GlobalContext* globalCtx) {
    if (this->unk_16E == 0) {
        if (this->dyna.unk_160 != 0) {
            this->unk_16E++;
            this->unk_170 = 20;

            if ((s16)(this->dyna.actor.rotTowardsLinkY + 0x4000) < 0) {
                this->unk_174 = -1000;
            } else {
                this->unk_174 = 1000;
            }

            if (this->dyna.actor.rotTowardsLinkY < 0) {
                this->unk_172 = -this->unk_174;
            } else {
                this->unk_172 = this->unk_174;
            }
        }
    } else {
        if (this->unk_168 != 0) {
            this->unk_168--;
        } else {
            this->dyna.actor.shape.rot.y += this->unk_172;
            this->dyna.actor.shape.rot.x += this->unk_174;
            this->unk_170--;
            this->dyna.actor.gravity = -1.0f;

            if (this->unk_170 == 0) {
                this->dyna.actor.posRot.pos = this->dyna.actor.initPosRot.pos;
                this->unk_16E = 0;
                this->unk_168 = 10;
                this->dyna.actor.velocity.y = 0.0f;
                this->dyna.actor.gravity = 0.0f;
                this->dyna.actor.shape.rot = this->dyna.actor.posRot.rot;
            }
        }
    }
}

void func_8001D480(EnAObj* this, s16 params) {
    EnAObj_SetupAction(this, (ActorFunc)func_8001D4A8);
}

void func_8001D4A8(EnAObj* this, GlobalContext* globalCtx) {
    Math_SmoothScaleMaxMinF(&this->dyna.actor.speedXZ, 1.0f, 1.0f, 0.5f, 0.0f);
    this->dyna.actor.shape.rot.x = this->dyna.actor.shape.rot.x + (this->dyna.actor.posRot.rot.x >> 1);
    this->dyna.actor.shape.rot.z = this->dyna.actor.shape.rot.z + (this->dyna.actor.posRot.rot.z >> 1);

    if ((this->dyna.actor.speedXZ != 0.0f) && (this->dyna.actor.bgCheckFlags & 0x8)) {
        if (1) { // Necessary to match
            this->dyna.actor.posRot.rot.y =
                ((this->dyna.actor.unk_7E - this->dyna.actor.posRot.rot.y) + this->dyna.actor.unk_7E) - 0x8000;
        }
        this->dyna.actor.bgCheckFlags &= ~0x8;
    }

    if (this->dyna.actor.bgCheckFlags & 0x2) {
        if (this->dyna.actor.velocity.y < -8.0f) {
            this->dyna.actor.velocity.y *= -0.6f;
            this->dyna.actor.speedXZ *= 0.6f;
            this->dyna.actor.bgCheckFlags &= ~0x3;
        } else {
            Actor_Kill(&this->dyna.actor);
        }
    }
}

void func_8001D5C8(EnAObj* this, s16 params) {
    this->dyna.actor.unk_FC = 1200.0f;
    this->dyna.actor.unk_F8 = 720.0f;
    EnAObj_SetupAction(this, (ActorFunc)func_8001D608);
}

void func_8001D608(EnAObj* this, GlobalContext* globalCtx) {
    this->dyna.actor.speedXZ += this->dyna.unk_150;
    this->dyna.actor.posRot.rot.y = this->dyna.unk_158;

    this->dyna.actor.speedXZ = (this->dyna.actor.speedXZ < -2.5f)
                                   ? -2.5f
                                   : ((this->dyna.actor.speedXZ > 2.5f) ? 2.5f : this->dyna.actor.speedXZ);

    Math_SmoothScaleMaxMinF(&this->dyna.actor.speedXZ, 0.0f, 1.0f, 1.0f, 0.0f);

    if (this->dyna.actor.speedXZ != 0.0f) {
        Audio_PlayActorSound2(&this->dyna.actor, 0x200A);
    }

    this->dyna.unk_154 = 0.0f;
    this->dyna.unk_150 = 0.0f;
}

void EnAObj_Update(EnAObj* this, GlobalContext* globalCtx) {
    Collider* collider;

    this->actionFunc(this, globalCtx);
    Actor_MoveForward(&this->dyna.actor);

    if (this->dyna.actor.gravity != 0.0f) {
        if (this->dyna.actor.params != A_OBJ_KNOB) {
            func_8002E4B4(globalCtx, &this->dyna.actor, 5.0f, 40.0f, 0.0f, 0x1D);
        } else {
            func_8002E4B4(globalCtx, &this->dyna.actor, 5.0f, 20.0f, 0.0f, 0x1D);
        }
    }

    this->dyna.actor.posRot2.pos = this->dyna.actor.posRot.pos;
    this->dyna.actor.posRot2.pos.y += this->unk_178;

    switch (this->dyna.actor.params) {
        case A_OBJ_SIGNPOST_OBLONG:
        case A_OBJ_SIGNPOST_ARROW:
            collider = (Collider*)&this->collider;
            Collider_CylinderUpdate(&this->dyna.actor, &this->collider);
            CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, collider);
    }
}

void EnAObj_Draw(EnAObj* this, GlobalContext* globalCtx) {
    s32 type = this->dyna.actor.params;
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    Gfx* dispRefs[4];

    Graph_OpenDisps(dispRefs, globalCtx->state.gfxCtx, "../z_en_a_keep.c", 701);

    func_80093D18(globalCtx->state.gfxCtx);

    if (type > A_OBJ_KNOB) {
        type = A_OBJ_KNOB;
    }

    if (this->dyna.actor.params == A_OBJ_KNOB) {
        gDPSetPrimColor(gfxCtx->polyOpa.p++, 0, 1, 0x3C, 0x3C, 0x3C, 0x32);
    }

    gSPMatrix(gfxCtx->polyOpa.p++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_a_keep.c", 712),
              G_MTX_MODELVIEW | G_MTX_LOAD);
    gSPDisplayList(gfxCtx->polyOpa.p++, D_80115484[type]);

    Graph_CloseDisps(dispRefs, globalCtx->state.gfxCtx, "../z_en_a_keep.c", 715);
}

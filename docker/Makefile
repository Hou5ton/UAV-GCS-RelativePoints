IMAGES := $(wildcard apx-*)

# ACTIONS_GOALS := $(filter-out $(IMAGES),$(MAKECMDGOALS))
# ACTIONS := $(if $(ACTIONS_GOALS),$(ACTIONS_GOALS),build)

# IMAGES_GOALS := $(filter $(IMAGES),$(MAKECMDGOALS))
# DEFAULT_GOALS := $(if $(IMAGES_GOALS),,$(foreach a,$(ACTIONS),$(IMAGES:%=%-$(a))))

# all: $(IMAGES:%=%-build)



# $(IMAGES): FORCE
# 	@make $(foreach a,$(ACTIONS),$@-$(a))

$(IMAGES): FORCE
	@echo "Building docker image $@..."
	@docker build -t uavos/$@ $@

%-run: FORCE
	@echo "Running docker image $*..."
	@docker run -it -v $(CURDIR)/..:/apx -w /apx --privileged uavos/$* bash

%-push: FORCE
	@echo "Pushing docker image $*..."
	@docker push uavos/$*


apx-gcs-linux:
	@docker build -t uavos/apx-linux apx-linux
	@docker stop $@; docker rm -f $@; echo ""
	@docker run -i --name $@ -v $(CURDIR)/..:/apx -w /apx uavos/apx-linux sudo make tools install-tools-gcs
	@docker commit $@ uavos/$@
	@docker run -i uavos/$@ rm -rf /var/cache/apt
	@docker commit $@ uavos/$@

apx-fw-linux:
	@docker build -t uavos/apx-linux apx-linux
	@docker stop $@; docker rm -f $@; echo ""
	@docker run -i --name $@ -v $(CURDIR)/..:/apx -w /apx uavos/apx-linux sudo make tools install-tools-fw
	@docker commit $@ uavos/$@
	@docker run -i uavos/$@ sudo rm -rf /var/cache/apt
	@docker commit $@ uavos/$@

apx-docs-linux:
	@docker build -t uavos/apx-linux apx-linux
	@docker stop $@ || echo ""; docker rm -f $@ || echo ""
	@docker run -i --name $@ -v $(CURDIR)/..:/apx -w /apx uavos/apx-linux sudo make tools install-tools-docs
	@docker commit $@ uavos/$@
	@docker run -i uavos/$@ sudo rm -rf /var/cache/apt
	@docker commit $@ uavos/$@




.PHONY: FORCE


# subdir target catch
%: $(DEFAULT_GOALS)
	@echo "Finished: $*"
#
